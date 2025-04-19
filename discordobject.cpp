/**
 * @file      discordobject.cpp
 * @author    Jason Howard <jth@howardlogic.com>
 * @copyright Howard Logic.  All rights reserved.
 * 
 * See LICENSE file in the project root folder for license information
 */

// FIXME: This is really hacky as it causes the implementation to get compiled as part of this
// source file.  The Discord includes should either inline their implementation, put the implementation
// in the class declaration, or provide a standalone CPP source we should compile (or compile that
// in their library).  I expect they had a reason not to do the last one, I haven't looked 
// further --jth
//
// Note: this needs to be above the include of the discordobject.h header, since that's where 
// discordpp.h is included.
#define DISCORDPP_IMPLEMENTATION

#include "discordobject.h"
#include <QTimer>
#include <QtDebug>

// Replace with your Discord Application ID
//const uint64_t APPLICATION_ID = 1362903946099949889;
const uint64_t APPLICATION_ID = 1362900698316013760; // SocialSDKTest1

DiscordObject::DiscordObject(QObject *p) : 
    QObject(p)
{
    setup();
}

bool DiscordObject::setup() {
    if(_housekeepingTimer != nullptr) {
        qWarning("setup called, but already setup");
        return true;
    }

    // Add logging callback to the client
    // FIXME: Really should map the severity to the Qt logger severity
    client.AddLogCallback(
        [](auto message, auto severity) {
            qInfo("[%s] %s", EnumToString(severity), message.c_str());
        }, 
        discordpp::LoggingSeverity::Verbose
    );

    client.SetStatusChangedCallback(
        [&](discordpp::Client::Status status, discordpp::Client::Error error, int32_t errorDetail) {
            qInfo() << "Status changed: " << discordpp::Client::StatusToString(status).c_str();
            if(status == discordpp::Client::Status::Ready) {
                qInfo("Client is ready! You can now call SDK functions.");
                emit ready();
            } else if(error != discordpp::Client::Error::None) {
                qInfo("Connection Error: %s, code %d", discordpp::Client::ErrorToString(error).c_str(), errorDetail);
            }        
    });

    _housekeepingTimer = new QTimer(this);
    connect(_housekeepingTimer, &QTimer::timeout, this, &DiscordObject::housekeeping);
    _housekeepingTimer->start(10);  // The discord docs suggest a 10ms housekeeping time
    return true;
}

bool DiscordObject::auth() {
    if(_housekeepingTimer == nullptr) {
        qWarning("auth called before setup");
        return false;
    }

    auto codeVerifier = client.CreateAuthorizationCodeVerifier();

    // Set up authentication arguments
    discordpp::AuthorizationArgs args{};
    args.SetClientId(APPLICATION_ID);
    args.SetScopes(discordpp::Client::GetDefaultPresenceScopes());
    args.SetCodeChallenge(codeVerifier.Challenge());

    // Begin authentication process
    client.Authorize(args, 
        [&, codeVerifier](auto result, auto code, auto redirectUri) {
            qInfo("Redirect URI: %s", redirectUri.c_str());
            if(!result.Successful()) {
                qWarning("Authentication Error: %s", result.Error().c_str());
                return;
            } else {
                qInfo("Authorization successful! Getting access token...");

                // exchange auth code for access token
                client.GetToken(APPLICATION_ID, code, codeVerifier.Verifier(), redirectUri,
                    [&](discordpp::ClientResult result, std::string accessToken, std::string refreshToken, 
                       discordpp::AuthorizationTokenType tokenType, int32_t expiresIn, std::string scope) {
                        qInfo("Access token received! Establishing connection...");
                        client.UpdateToken(discordpp::AuthorizationTokenType::Bearer,  accessToken, 
                            [&](discordpp::ClientResult result) {
                                if(result.Successful()) {
                                    qInfo("Token updated, connecting to Discord...");
                                    client.Connect();
                                }
                        });
                    });
                }
    });
    return true;
}

void DiscordObject::housekeeping() {
    discordpp::RunCallbacks();
    return;
}

