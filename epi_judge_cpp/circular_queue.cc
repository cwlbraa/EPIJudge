#include "test_framework/generic_test.h"
#include "test_framework/serialization_traits.h"
#include "test_framework/test_failure.h"
#include <vector>

std::ostream& operator<<(std::ostream& os, const std::vector<int>& dt) {
  os << "[";
  for (auto i : dt) os << i << ", ";
  os << "]";
  return os;
}

class Queue {
  int tail = 0;
  int head = 0;
  int size = 0;
  int cap;
  std::vector<int> data;
public:
  Queue(size_t capacity)
    : data(capacity)
    , cap(capacity)
  {}

  void Enqueue(int x) {
    if (size + 1 == cap) {
      std::rotate(data.begin(), data.begin() + head, data.end());
      tail = size;
      head = 0;

      cap *= 2;
      data.resize(cap);
    }

    data[tail] = x;
    tail = (tail + 1) % cap;
    size++;
  }

  int Dequeue() {
    int rez = data[head];
    head = (head + 1) % cap;
    size--;
    return rez;
  }
  int Size() const {
    return size;
  }
};
struct QueueOp {
  enum { kConstruct, kDequeue, kEnqueue, kSize } op;
  int argument;

  QueueOp(const std::string& op_string, int arg) : argument(arg) {
    if (op_string == "Queue") {
      op = kConstruct;
    } else if (op_string == "dequeue") {
      op = kDequeue;
    } else if (op_string == "enqueue") {
      op = kEnqueue;
    } else if (op_string == "size") {
      op = kSize;
    } else {
      throw std::runtime_error("Unsupported queue operation: " + op_string);
    }
  }

  void execute(Queue& q) const {
    switch (op) {
      case kConstruct:
        // Hack to bypass deleted assign operator
        q.~Queue();
        new (&q) Queue(argument);
        break;
      case kDequeue: {
        int result = q.Dequeue();
        if (result != argument) {
          throw TestFailure("Dequeue: expected " + std::to_string(argument) +
                            ", got " + std::to_string(result));
        }
      } break;
      case kEnqueue:
        q.Enqueue(argument);
        break;
      case kSize: {
        int s = q.Size();
        if (s != argument) {
          throw TestFailure("Size: expected " + std::to_string(argument) +
                            ", got " + std::to_string(s));
        }
      } break;
    }
  }
};

template <>
struct SerializationTraits<QueueOp> : UserSerTraits<QueueOp, std::string, int> {
};

void QueueTester(const std::vector<QueueOp>& ops) {
  Queue q(0);
  for (auto& op : ops) {
    op.execute(q);
  }
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"ops"};
  return GenericTestMain(args, "circular_queue.cc", "circular_queue.tsv",
                         &QueueTester, DefaultComparator{}, param_names);
}
