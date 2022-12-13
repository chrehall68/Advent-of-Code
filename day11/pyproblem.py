from typing import *
from tqdm import tqdm


class Monkey:
    def __init__(self) -> None:
        self.items = []
        self.op = lambda x: x
        self.divisor = 1
        self.give_to_true: Monkey = None
        self.give_to_false: Monkey = None
        self.num_inspected = 0

    def inspect(self):
        for i in range(len(self.items)):
            self.items[i] = self.op(self.items[i])
            self.num_inspected += 1

    def test(self):
        for num in self.items:
            if num % self.divisor == 0:
                self.give_to_true.items.append(num)
            else:
                self.give_to_false.items.append(num)
        self.items.clear()


def read_monkey(file, monkey_lst):
    line = file.readline()
    line = line.strip("Monkey ")
    monkey_num = int(line[0])
    print("monkey", monkey_num)

    # starting items
    line = file.readline()
    line = line.strip("Starting items: ")
    nums = line.split(", ")
    nums = [int(nums[i]) for i in range(len(nums))]
    monkey_lst[monkey_num].items = nums
    print("starting items:", nums)

    # operation
    line = file.readline()
    line = line.strip("Operation: new = old ")
    operation = line[0]
    operand = line.strip(operation).strip()
    op = lambda x: x
    if operand[0] == "o":
        if operation == "+":
            op = lambda x: x + x
        if operation == "*":
            op = lambda x: x * x
    else:
        operand = int(operand)
        if operation == "+":
            op = lambda x: x + operand
        if operation == "*":
            op = lambda x: x * operand
    monkey_lst[monkey_num].op = op
    print("operation is x ", operation, operand)

    # test
    line = file.readline()
    div = int(line.strip("Test: divisible by "))
    monkey_lst[monkey_num].divisor = div
    print("divisor is", div)

    # tru and false
    line = file.readline().strip()
    true_monkey = int(line[-1])
    line = file.readline().strip()
    false_monkey = int(line[-1])
    print("if true, throw to ", true_monkey, "else throw to", false_monkey)
    monkey_lst[monkey_num].give_to_true = monkey_lst[true_monkey]
    monkey_lst[monkey_num].give_to_false = monkey_lst[false_monkey]

    # remove extra
    line = file.readline()


def read_monkeys(file_name, num_monkeys) -> List[Monkey]:
    monkey_list = [Monkey() for i in range(num_monkeys)]
    with open(file_name, "r") as file:
        for i in range(num_monkeys):
            read_monkey(file, monkey_list)

    return monkey_list


if __name__ == "__main__":
    monkeys = read_monkeys("day11//day11-input.txt", 4)
    rounds = 1000
    for round in tqdm(range(rounds)):
        for monkey in monkeys:
            monkey.inspect()
            monkey.test()

    for monkey in monkeys:
        print("monkey had num inspected", monkey.num_inspected)
