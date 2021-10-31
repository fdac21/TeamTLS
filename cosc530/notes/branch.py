#!/usr/bin/env python3

class CorrelationPredictor():
    counter = 0

    def __init__(self, m, n, state, init=None):
        self.m, self.bits = m, n
        if init:
            self.state = {
                br: {
                    "shift": ["0" for __ in range(m)],
                    "predictors": [state for _ in range(2 ** m)],
                    "misses": 0,
                    "hits": 0,
                } for br, state in init.items()
            }
        else:
            self.state = {
                br: {
                    "shift": ["0" for __ in range(m)],
                    "predictors": [[0 for __ in range(n)] for _ in range(2 ** m)],
                    "misses": 0,
                    "hits": 0,
                } for br in state
            }

    def getPIndex(self, buffer=""):
        if buffer == "":
            raise ValueError
        if (self.m == 0):
            return 0
        return int("".join(self.state[buffer]["shift"]), base=2)

    def predict(self, buffer=""):
        if buffer == "":
            raise ValueError
        index = self.getPIndex(buffer)
        try:
            return bool(self.state[buffer]["predictors"][index][0])
        except:
            print(self.state[buffer])

    def updateHistory(self, taken, buffer=""):
        if (self.m > 0):
            # self.state[buffer]["shift"] = self.state[buffer]["shift"][1:] + \
            #     ["1" if taken else "0"]
            self.state[buffer]["shift"] = ["1" if taken else "0"] + \
                self.state[buffer]["shift"][:-1]

    def incrementHits(self, taken, buffer=""):
        self.state[buffer]["hits"] += 1
        self.updateHistory(taken, buffer)

    def incrementMisses(self, taken, buffer=""):
        self.state[buffer]["misses"] += 1
        self.updateHistory(taken, buffer)

    def updateState(self, buffer="", taken=True):
        if buffer == "":
            raise ValueError
        index = self.getPIndex(buffer)
        res = self.predict(buffer)

        if (buffer == "b1"):
            print(buffer, self.state[buffer]["shift"],
                  self.state[buffer]["predictors"], "hit" if res == taken else "miss")

        if self.bits == 1:
            a, b = [0], [1]

            if self.state[buffer]["predictors"][index] == a:
                if not taken:
                    self.incrementHits(False, buffer)
                else:
                    self.state[buffer]["predictors"][index] = b
                    self.incrementMisses(True, buffer)
            elif self.state[buffer]["predictors"][index] == b:
                if taken:
                    self.incrementHits(True, buffer)
                else:
                    self.incrementMisses(False, buffer)
                    self.state[buffer]["predictors"][index] = a
        elif self.bits == 2:
            a, b, c, d = [0, 0], [0, 1], [1, 0], [1, 1]

            if self.state[buffer]["predictors"][index] == a:
                if not taken:
                    self.incrementHits(False, buffer)
                else:
                    self.state[buffer]["predictors"][index] = b
                    self.incrementMisses(True, buffer)
            elif self.state[buffer]["predictors"][index] == b:
                if not taken:
                    self.incrementHits(False, buffer)
                    self.state[buffer]["predictors"][index] = a
                else:
                    self.incrementMisses(True, buffer)
                    self.state[buffer]["predictors"][index] = d
            elif self.state[buffer]["predictors"][index] == c:
                if taken:
                    self.incrementHits(True, buffer)
                    self.state[buffer]["predictors"][index] = d
                else:
                    self.incrementMisses(False, buffer)
                    self.state[buffer]["predictors"][index] = a
            elif self.state[buffer]["predictors"][index] == d:
                if taken:
                    self.incrementHits(True, buffer)
                else:
                    self.incrementMisses(False, buffer)
                    self.state[buffer]["predictors"][index] = c


def simulate(predictor):
    for i in range(1, 101):
        if i < 101:
            predictor.updateState("b3", taken=True)
        if i % 4 == 0:
            predictor.updateState("b1", taken=False)
            for j in range(1, 11):
                predictor.updateState("b2", taken=True)
            else:
                predictor.updateState("b2", taken=False)
        else:
            predictor.updateState("b1", taken=True)
    else:
        predictor.updateState("b3", taken=False)

    for k, v in predictor.state.items():
        print(f"{k} - Misses: {v['misses']}\t Hits: {v['hits']}")


def main():
    init_cp1 = {
        "b1": [1],
        "b2": [0],
        "b3": [0]
    }
    branches = ["b1", "b2", "b3"]

    predictor_1bit = CorrelationPredictor(0, 1, branches, init_cp1)

    print("Simulating 1-bit branch prediction buffer")
    simulate(predictor_1bit)

    init_2bit = {
        "b1": [0, 1],
        "b2": [1, 0],
        "b3": [1, 0],
    }
    predictor_2bit = CorrelationPredictor(0, 2, branches, init_2bit)

    print("Simulating 2-bit branch prediction buffer")
    simulate(predictor_2bit)

    CBP_1 = CorrelationPredictor(1, 2, branches)
    print("Simulating Correlating (1, 2) branch prediction buffer")
    simulate(CBP_1)

    CBP_2 = CorrelationPredictor(2, 1, branches)
    print("Simulating Correlating (2, 1) branch prediction buffer")
    simulate(CBP_2)


if __name__ == '__main__':
    main()
