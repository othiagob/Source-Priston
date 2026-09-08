#pragma once

typedef void (*ServerStatusFn)(const char* msg);

void ServerConfig_Draw(ServerStatusFn setStatus);
void ServerConfig_ReloadFromDisk();
