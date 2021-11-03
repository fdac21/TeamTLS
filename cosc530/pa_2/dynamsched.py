#!/usr/bin/env python3.6
import sys


class ResStation():
    pass


class Entry():
    pass


class ReorderBuffer():
    def __init__(self, size):
        self.size = size
        self.buffer = []


class ResStationBuffer():
    def __init__(self, size):
        # TODO: Might remove size attribute
        self.size = size
        self.buffer = [ResStation() for _ in range(size)]


class Register():
    def __init__(self, index):
        self.index = index
        ResStation


class DynamSched():

    def __init__(self, configFile):
        with open(configFile) as conf:
            options = [opt.split(':')
                       for opt in conf.readlines() if ':' in opt]

            self.buffers = {
                k.strip(): int(v.strip()) for k, v in options[:5]
            }
            self.latencies = {
                k.strip(): int(v.strip()) for k, v in options[5:]
            }
            print(self.latencies)

            self.buffers = {
                k: {
                    "size": v,
                    "buffer": ResStationBuffer(v)
                } for k, v in self.buffers.items()
            }

            # for k, v in self.buffers.items():
            #     print(k, v["size"])

    def process(self):
        for line in sys.stdin:
            print(line.strip())


def main():
    configFile = "source/config.txt"
    sched = DynamSched(configFile)
    sched.process()


if __name__ == '__main__':
    main()
