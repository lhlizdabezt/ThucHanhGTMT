//gcc TEST.c sqlite3.c -o App.exe
//.\App.exe
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdint.h>
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#include "sqlite3.h" // <--- THƯ VIỆN QUAN TRỌNG

#define BUFFER_SIZE 256

volatile int g_sensor_period = 5000;
volatile int g_ble_period = 500;
volatile int is_logging_mode = 0;

// --- CẤU HÌNH CỔNG COM ---
// Đổi thành cổng COM của bạn (Ví dụ COM3, COM6...)
const char* COM_PORT_NAME = "\\\\.\\COM10"; 

// =============================================================
//               PHẦN XỬ LÝ SQLITE DATABASE
// =============================================================

// Hàm tạo Database và Bảng nếu chưa có
int init_database(sqlite3 **db) {
    int rc = sqlite3_open("sensor_data.db", db);
    if (rc != SQLITE_OK) {
        printf("Khong the mo database: %s\n", sqlite3_errmsg(*db));
        return 0; // Lỗi
    }

    char *err_msg = 0;
    // Tạo bảng: ID (tự tăng), Thời gian, Nhiệt độ, Độ ẩm, Chu kỳ
    const char *sql = "CREATE TABLE IF NOT EXISTS SensorLog ("
                      "ID INTEGER PRIMARY KEY AUTOINCREMENT, "
                      "Timestamp TEXT, "
                      "Temperature REAL, "
                      "Humidity REAL, "
                      "SensorPeriod INT, "
                      "BLEPeriod INT);";

    rc = sqlite3_exec(*db, sql, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        printf("SQL Error (Tao bang): %s\n", err_msg);
        sqlite3_free(err_msg);
        return 0; // Lỗi
    }
    return 1; // Thành công
}

// Hàm lưu dữ liệu vào Database (Thay cho save_to_csv)
void save_to_sqlite(sqlite3 *db, const char* timestamp, float temp, float hum) {
    char sql[512];
    char *err_msg = 0;

    // Tạo câu lệnh INSERT
    sprintf(sql, "INSERT INTO SensorLog (Timestamp, Temperature, Humidity, SensorPeriod, BLEPeriod) "
                 "VALUES ('%s', %.2f, %.2f, %d, %d);", 
                 timestamp, temp, hum, g_sensor_period, g_ble_period);

    // Thực thi lệnh
    int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    
    if (rc != SQLITE_OK) {
        printf(" -> [SQL Loi]: %s\n", err_msg);
        sqlite3_free(err_msg);
    } else {
        printf(" -> [Saved to DB]");
    }
}

// =============================================================
//               PHẦN GIAO TIẾP UART (GIỮ NGUYÊN)
// =============================================================

HANDLE open_port(const char* device, unsigned long baud_rate) {
    HANDLE port = CreateFileA(device, GENERIC_READ | GENERIC_WRITE, 0, NULL,
        OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (port == INVALID_HANDLE_VALUE) return INVALID_HANDLE_VALUE;

    PurgeComm(port, PURGE_RXCLEAR | PURGE_TXCLEAR);

    COMMTIMEOUTS timeouts = { 0 };
    timeouts.ReadIntervalTimeout = 10;
    timeouts.ReadTotalTimeoutConstant = 10; 
    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;
    SetCommTimeouts(port, &timeouts);

    DCB dcb = { 0 };
    dcb.DCBlength = sizeof(DCB);
    GetCommState(port, &dcb);
    dcb.BaudRate = baud_rate;
    dcb.ByteSize = 8;
    dcb.Parity = NOPARITY;
    dcb.StopBits = ONESTOPBIT;
    dcb.fDtrControl = DTR_CONTROL_ENABLE;
    dcb.fRtsControl = RTS_CONTROL_ENABLE;
    SetCommState(port, &dcb);
    return port;
}

int uart_transmit(HANDLE port, const char* data) {
    DWORD written = 0;
    if (!WriteFile(port, data, strlen(data), &written, NULL)) return -1;
    return 0;
}

void get_timestamp(char* buf) {
    time_t now = time(NULL);
    struct tm* t = localtime(&now);
    sprintf(buf, "%04d-%02d-%02d %02d:%02d:%02d", 
            t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
            t->tm_hour, t->tm_min, t->tm_sec);
}

// =============================================================
//                       HÀM MAIN
// =============================================================

int main() {
    // 1. KẾT NỐI DATABASE
    sqlite3 *db;
    if (!init_database(&db)) {
        system("pause");
        return -1;
    }

    // 2. KẾT NỐI COM
    HANDLE port = open_port(COM_PORT_NAME, 115200);
    if (port == INVALID_HANDLE_VALUE) {
        printf("Loi mo cong COM: %s\n", COM_PORT_NAME);
        sqlite3_close(db); // Nhớ đóng DB trước khi thoát
        system("pause");
        return -1;
    }

    printf("Da ket noi Database & COM. Cho Board khoi dong (2s)...\n");
    Sleep(2000);
    printf("San sang!\n");

    char rx_buffer[BUFFER_SIZE];
    int rx_idx = 0;
    char c_rx;
    DWORD bytes_read;
    char cmd_buffer[50];

    uart_transmit(port, "STOP\r"); // Gửi lệnh dừng ban đầu

    while (1) {
        // --- MENU ---
        if (!is_logging_mode) {
            system("cls");
            printf("=== HE THONG GIAM SAT SQLITE ===\n");
            printf("  1. Chu ky cam bien (%d ms)\n", g_sensor_period);
            printf("  2. Chu ky BLE (%d ms)\n", g_ble_period);
            printf("  R. BAT DAU (Run)\n");
            printf("  Esc. Thoat\n");
            printf("> ");

            char choice = _getch();
            if (choice == '1') {
                printf("\nNhap ms: "); scanf("%s", cmd_buffer);
                g_sensor_period = atoi(cmd_buffer);
                sprintf(cmd_buffer, "S:%d\r", g_sensor_period);
                uart_transmit(port, cmd_buffer);
                Sleep(500);
            }
            else if (choice == '2') {
                printf("\nNhap ms: "); scanf("%s", cmd_buffer);
                g_ble_period = atoi(cmd_buffer);
                sprintf(cmd_buffer, "B:%d\r", g_ble_period);
                uart_transmit(port, cmd_buffer);
                Sleep(500);
            }
            else if (choice == 'r' || choice == 'R') {
                uart_transmit(port, "START\r");
                is_logging_mode = 1;
                printf("\nDang chay... Nhan 'S' de Dung.\n");
                Sleep(100);
            }
            else if (choice == 27) break;
        }
        // --- CHẠY LOGGING ---
        else {
            if (ReadFile(port, &c_rx, 1, &bytes_read, NULL) && bytes_read > 0) {
                if (c_rx == '\n' || c_rx == '\r') {
                    if (rx_idx > 0) {
                        rx_buffer[rx_idx] = '\0';
                        float temp, hum;
                        
                        // Parse: D:temp,hum
                        char *d_ptr = strstr(rx_buffer, "D:");
                        if (d_ptr != NULL) {
                            if (sscanf(d_ptr, "D:%f,%f", &temp, &hum) == 2) {
                                char time_buf[30];
                                get_timestamp(time_buf);
                                
                                // LƯU VÀO SQLITE
                                save_to_sqlite(db, time_buf, temp, hum);
                            }
                        }
                        rx_idx = 0;
                    }
                } else if (rx_idx < BUFFER_SIZE - 1) {
                    rx_buffer[rx_idx++] = c_rx;
                }
                printf("%c", c_rx); // Echo ra màn hình
            }

            if (_kbhit()) {
                char key = _getch();
                if (key == 's' || key == 'S' || key == 27) {
                    uart_transmit(port, "STOP\r");
                    is_logging_mode = 0;
                    printf("\nDang dung...\n");
                    Sleep(1000);
                }
            }
        }
    }

    // ĐÓNG KẾT NỐI
    sqlite3_close(db);
    CloseHandle(port);
    return 0;
}