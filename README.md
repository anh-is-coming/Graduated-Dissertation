Khóa luận tốt nghiệp: Nghiên cứu chế tạo hệ thống đa cảm biến ứng dụng cảnh báo độ chín trái cây 

Cấu tạo hệ thống thiết bị:
+ Sử dụng thùng kín để đặt trái cây cần phân tích và hệ cảm biến.

Hệ thống đa cảm biến gồm:
+ Cảm biến nhiệt độ, độ ẩm: DHT22 sử dụng giao thức 1 Wire
+ Cảm biến CO2: C8-CO2 sử dụng giao thức UART
+ Cảm biến Etilen: ZE03-C2H4 sử dụng giao thức UART
+ Module Camera: ESP32-CAM
+ Vi điều khiển: ESP32
  
Nguyên lý: 
+ Thu thập dữ liệu phân tích được từ hệ thống đa cảm biến trong toàn bộ quá trình quả từ xanh đến chín, sau đó đăng tải lên trang web ThingSpeak, từ đó người dùng có thể quan sát cũng như tải các dữ liệu để phục vụ mục đích nghiên cứu.
 
Git bao gồm:
+ File Code.
+ Hình ảnh hệ thống đa cảm biến/hệ đo/dữ liệu thu được đăng tải lên ThingSpeak.
