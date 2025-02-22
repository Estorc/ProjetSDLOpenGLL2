#pragma once

#include "color.h"
#define PRINT_ERROR(x, ...) fprintf(stderr, BOLD_RED "[ERROR] " x RESET , ##__VA_ARGS__);
#define PRINT_INFO(x, ...) fprintf(stdout, GREY "[INFO] " x RESET , ##__VA_ARGS__);
#define PRINT_WARNING(x, ...) fprintf(stderr, YELLOW "[WARNING] " x RESET , ##__VA_ARGS__);
#define PRINT_SERVER_INFO(x, ...) fprintf(stdout, BOLD_CYAN "[SERVER INFO] " x RESET , ##__VA_ARGS__);
#define PRINT_CLIENT_INFO(x, ...) fprintf(stdout, BOLD_MAGENTA "[CLIENT INFO] " x RESET , ##__VA_ARGS__);
#define PRINT_SUCCESS(x, ...) fprintf(stdout, BOLD_GREEN "[SUCCESS] " x RESET , ##__VA_ARGS__);