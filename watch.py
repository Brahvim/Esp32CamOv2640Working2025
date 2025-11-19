import cv2
import websocket
import numpy as np

print("Please enter the ESP32's IP address...!: ", end="")
s_espUrl = f"ws://{input()}:80/stream"


def on_message(p_wock, p_message):
    cv2.imshow(
        s_espUrl,
        cv2.imdecode(
            np.frombuffer(
                p_message,
                np.uint8
            ),
            cv2.IMREAD_COLOR
        )
    )
    cv2.waitKey(1)  # Refresh CV window.


def on_error(p_wock, p_error):
    print(f"WebSocket error!: {p_error}")


def on_close(p_wock, p_status, p_message):
    print(f"WebSocket closed w/ message=\"{p_message}\".")


def on_open(p_wock):
    print("Connected to a wock!")


websocket.WebSocketApp(
    s_espUrl,
    on_open=on_open,
    on_error=on_error,
    on_close=on_close,
    on_message=on_message,
).run_forever()
