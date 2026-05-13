            val pos = IndoorTrilateration.calculatePosition(p1, d1, p2, d2, p3, d3)
            if (pos != null) {
                houseMapView.setUserPosition(pos.x, pos.y)
                val room = rooms.find { it.contains(pos.x, pos.y) }
                if (room != null) {
                    tvCurrentRoom.text =
                        "Indoor: Vị trí hiện tại: ${room.name} (x=%.2f, y=%.2f)".format(pos.x, pos.y)
                } else {
                    tvCurrentRoom.text =
                        "Indoor: Ngoài vùng (x=%.2f, y=%.2f)".format(pos.x, pos.y)
                }
            }