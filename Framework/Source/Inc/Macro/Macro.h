#pragma once

#ifndef DISALLOW_COPY_AND_ASSIGN
#define DISALLOW_COPY_AND_ASSIGN(TypeName)    \
TypeName(TypeName&&) = delete;                \
TypeName(const TypeName&) = delete;           \
TypeName& operator=(TypeName&&) = delete;     \
TypeName& operator=(const TypeName) = delete;
#endif

// NOTE: LOG_F(atal) 사용 시 해당 로그 실행하면 앱 강제 종료됨.
#define LOG_I(fmt, ...) LogUtils::Info(__FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__)
#define LOG_W(fmt, ...) LogUtils::Warn(__FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__)
#define LOG_E(fmt, ...) LogUtils::Error(__FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__)
#define LOG_F(fmt, ...) LogUtils::Fatal(__FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__)

#define LOG_IF_I(cond, fmt, ...) if ((cond)) LogUtils::Info(__FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__)
#define LOG_IF_W(cond, fmt, ...) if ((cond)) LogUtils::Warn(__FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__)
#define LOG_IF_E(cond, fmt, ...) if ((cond)) LogUtils::Error(__FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__)
#define LOG_IF_F(cond, fmt, ...) if ((cond)) LogUtils::Fatal(__FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__)