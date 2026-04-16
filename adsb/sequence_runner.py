import threading
import signal
import streampu as spu
import adsb

class SequenceRunner:
    def __init__(self, sequence):
        self.sequence = sequence
        self._stop_event = threading.Event()
        self._thread = threading.Thread(target=self._run)
        self._original_sigint = signal.getsignal(signal.SIGINT)
        self.radio = [mdl for mdl in self.sequence.get_modules(adsb._cpp.Radio_USRP_float32)]

    def _run(self):
        try:
            self.sequence.exec()
                
        except Exception as e:
            print(f"Sequence stopped: {e}")

    def start(self):
        def handler(signum, frame):
            print("Ctrl+C detected, stopping sequence...")
            self.radio[0].done_flag = True

        signal.signal(signal.SIGINT, handler)
        self._thread.start()

    def stop(self):
        self._stop_event.set()
        self._thread.join()
        signal.signal(signal.SIGINT, self._original_sigint)
        print("Sequence stopped cleanly.")
