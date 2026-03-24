#include "App/AppHost.h"
#include "Utils/LogUtils.h"

#include "App.h"

int main(int argc, char* argv[])
{
	AppHost appHost;
	if (Result<void> result = appHost.Startup(); !result.IsSuccess())
	{
		LOG_E("FAILED_TO_STARTUP_APP_HOST");
		return -1;
	}

	App app;
	if (Result<void> result = appHost.Run(app); !result.IsSuccess())
	{
		LOG_E("FAILED_TO_RUN_APP_HOST");
		return -1;
	}

	if (Result<void> result = appHost.Shutdown(); !result.IsSuccess())
	{
		LOG_E("FAILED_TO_SHUTDOWN_APP_HOST");
		return -1;
	}

	return 0;
}