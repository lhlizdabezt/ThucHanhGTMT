# Thực hành Giao tiếp máy tính và Thu nhận dữ liệu

Repo này lưu source code, báo cáo và slide cho môn **Thực hành Giao tiếp máy tính và Thu nhận dữ liệu** tại Khoa Điện tử - Viễn thông, Trường Đại học Khoa học Tự nhiên - ĐHQG TP.HCM.

Đồ án chính là **hệ thống đo nhiệt độ và độ ẩm** sử dụng cảm biến AHT20, board Silicon Labs BLE SoC, LCD hiển thị tại thiết bị, quảng bá dữ liệu qua BLE advertising, truyền log qua UART và lưu dữ liệu ở phía máy tính bằng SQLite.

## Nội dung chính

- Firmware nhúng đọc cảm biến AHT20 qua I2C.
- Hiển thị nhiệt độ, độ ẩm và chu kỳ đo lên LCD.
- Đóng gói dữ liệu nhiệt độ/độ ẩm vào BLE advertising payload.
- Gửi dữ liệu dạng `D:temperature,humidity` qua UART/VCOM.
- Chương trình PC đọc UART trên Windows và lưu log vào SQLite.
- Báo cáo đồ án, slide thuyết trình và báo cáo Lab 6 về Bluetooth Low Energy Mesh.

## Cấu trúc thư mục

| Đường dẫn | Nội dung |
| --- | --- |
| `LONG1_2-20260513T142225Z-3-001/LONG1_2/` | Project firmware Silicon Labs BLE SoC, gồm logic AHT20, LCD, BLE advertising và UART log. |
| `TH GTMT-20260513T142230Z-3-001/TH GTMT/` | Chương trình PC bằng C/Win32 API để đọc UART và lưu dữ liệu cảm biến vào SQLite. |
| `22207056_report_DoAn.pdf` | Báo cáo đồ án hệ thống đo nhiệt độ và độ ẩm. |
| `22207056_report_DoAn (2).pptx` | Slide thuyết trình đồ án. |
| `22207056_report_lab6.pdf` | Báo cáo Lab 6: Bluetooth Low Energy - Mesh. |

## Firmware nhúng

Project firmware nằm tại:

```text
LONG1_2-20260513T142225Z-3-001/LONG1_2/LONG1_2.slcp
```

Các file source quan trọng:

- `app.c`: logic chính, đọc AHT20 theo timer, cập nhật LCD, cập nhật BLE advertising payload và in log qua UART.
- `aht20.c`, `aht20.h`: driver đọc cảm biến AHT20 qua I2C.
- `lcd_display.c`, `lcd_display.h`: điều khiển LCD và hiển thị dữ liệu đo.
- `custom_adv.c`, `custom_adv.h`: phần hỗ trợ đóng gói dữ liệu quảng bá BLE.

Môi trường phát triển:

- Simplicity Studio
- Gecko SDK / Simplicity SDK tương thích project
- GNU Arm Embedded Toolchain
- Board Silicon Labs BLE SoC có VCOM/UART
- Cảm biến AHT20 và LCD

Thư mục build `GNU ARM v12.2.1 - Default/` không được đưa lên Git vì đây là output sinh tự động. Khi mở project bằng Simplicity Studio, build lại để tạo file `.hex`, `.s37`, `.bin` hoặc các artifact nạp firmware tương ứng.

## Chương trình PC

Source chương trình PC nằm tại:

```text
TH GTMT-20260513T142230Z-3-001/TH GTMT/TEST.c
```

Chức năng chính:

- Mở cổng COM trên Windows bằng Win32 API.
- Nhận dữ liệu UART từ board.
- Parse dữ liệu cảm biến dạng `D:temp,hum`.
- Lưu dữ liệu vào bảng SQLite `SensorLog`.
- Có menu console để cấu hình chu kỳ và điều khiển chế độ log.

Build nhanh bằng GCC trên Windows:

```powershell
cd "TH GTMT-20260513T142230Z-3-001\TH GTMT"
gcc TEST.c sqlite3.c -o App.exe
.\App.exe
```

Trước khi chạy, chỉnh lại cổng COM trong `TEST.c`:

```c
const char* COM_PORT_NAME = "\\\\.\\COM10";
```

Đổi `COM10` thành cổng VCOM thực tế của board trên máy đang dùng.

## Tài liệu

- `22207056_report_DoAn.pdf`: mô tả mục tiêu, nguyên lý hoạt động, thiết kế hệ thống và kết quả đồ án.
- `22207056_report_DoAn (2).pptx`: slide trình bày đồ án.
- `22207056_report_lab6.pdf`: bài thực hành BLE Mesh, gồm truyền MSSV qua mesh và truyền trạng thái LED/thời gian hoạt động.

## Ghi chú về repo

Repo chỉ version source, project config và tài liệu cần thiết. Các file runtime/build như `.exe`, `.dll`, `.db`, `.o`, `.map`, `.hex`, `.bin` và thư mục build của toolchain đã được ignore để tránh làm repo nặng và khó bảo trì.

## Thông tin

- Môn học: Thực hành Giao tiếp máy tính và Thu nhận dữ liệu
- Đề tài đồ án: Hệ thống đo nhiệt độ và độ ẩm
- Sinh viên: Lương Hải Long
- MSSV: 22207056
- Lớp: 22DTV_CLC1
