# OpenBattle Dev Log — 2026-10-07

## Mục tiêu phiên hôm nay
- Nâng chất lượng scene OpenBattle theo hướng chân thực hơn, giảm cảm giác "nhựa" và gamey.
- Tập trung vào khu tầng 1 và cầu thang nối tầng 1 → tầng 2.

## Đã thực hiện
- Dựng lại cầu thang tầng 1 → tầng 2:
  - 25 bậc đều.
  - Có chiếu nghỉ dưới và trên.
  - Có nẹp chống trượt ở mép bậc.
  - Có stringer hai bên.
  - Tay vịn chạy liên tục theo độ dốc, có trụ đứng và thanh giữa.
- Cập nhật vật liệu cầu thang:
  - Bê tông nhám cho bậc/chiếu nghỉ.
  - Kim loại sơn tĩnh điện cho lan can/tay vịn.
  - Nẹp chống trượt tối màu.
- Kiểm tra lại vị trí cầu thang so với sàn tầng 2.
- Phát hiện cầu thang cũ đặt lệch, một phần chui vào mép sàn tầng 2.
- Đã dịch toàn bộ cụm cầu thang/lan can khoảng +3m theo trục X, đưa tâm cầu thang từ x≈6m sang x≈9m để nằm đúng giữa khoảng mở tầng 2.
- Đã lưu scene Blender hiện tại vào `Generated/Blender/openbattle_tower.blend`.

## File tham chiếu
- `Generated/Blender/openbattle_tower.blend` — bản scene hiện tại.
- `Generated/Blender/build_openbattle_tower.py` — script dựng scene.

## Việc cần làm tiếp
- Review lại vị trí cầu thang bằng camera/viewport để xác nhận đầu thang ăn đúng sàn tầng 2 và luồng di chuyển hợp lý.
- Chỉnh phần còn lại của tầng 1 theo hướng photorealistic hơn:
  - sàn,
  - tường,
  - cửa,
  - trần,
  - các phòng,
  - ánh sáng,
  - khu dân cư/ngoại cảnh.
- Giảm cảm giác vật liệu quá phẳng/nhựa bằng roughness, bump/normal, edge wear và ánh sáng contact shadow hợp lý.
- Thay các nhân vật khối thô bằng silhouette/proxy người có tỷ lệ tự nhiên hơn.

## Ghi chú handoff
Scene hiện tại được giữ làm mốc để team có thể pull về và tiếp tục từ đúng trạng thái hôm nay. Không dùng file `openbattle_tower.blend1` làm file chính; đó chỉ là backup tự động của Blender.
