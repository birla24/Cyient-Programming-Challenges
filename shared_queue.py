import threading
import time

NUM_READERS = 5
NUM_MESSAGES = 25

class SharedQueue:
    def __init__(self, capacity):
        self.messages = []
        self.capacity = capacity
        self.lock = threading.Lock()
        self.not_full = threading.Condition(self.lock)
        self.not_empty = threading.Condition(self.lock)

    def enqueue(self, message):
        with self.not_full:
            while len(self.messages) >= self.capacity:
                self.not_full.wait()
            self.messages.append(message)
            self.not_empty.notify_all()

    def dequeue(self):
        with self.not_empty:
            while not self.messages:
                self.not_empty.wait()
            message = self.messages.pop(0)
            self.not_full.notify()
            return message

def writer_thread(queue):
    for i in range(1, NUM_MESSAGES + 1):
        message = f"Message {i}"
        queue.enqueue(message)
        time.sleep(0.2)  # Add 5 messages per second

def reader_thread(queue, reader_id):
    while True:
        message = queue.dequeue()
        if message is None:  # No more messages in the queue
            break
        print(f"Reader {reader_id} received: {message}")

def main():
    queue = SharedQueue(NUM_READERS * 2)  # Capacity should be at least double the number of readers

    writer = threading.Thread(target=writer_thread, args=(queue,))
    writer.start()

    readers = [threading.Thread(target=reader_thread, args=(queue, i)) for i in range(NUM_READERS)]
    for reader in readers:
        reader.start()

    writer.join()
    for reader in readers:
        reader.join()

if __name__ == "__main__":
    main()
