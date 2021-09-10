class LRU:
    def __init__(self, m=3):
        self.max = m
        self.data = []

    def read(self, d):

        # Read condition
        if self.data.count(d):
            # Remove if found
            self.data.remove(d)
        # Prepend block pointer
        self.data.insert(0, d)
        # Remove last element
        self.data = self.data[:self.max]

l = LRU()

l.read(3)
l.read(2)
l.read(2)
l.read(1)
l.read(4)
l.read(4)
l.read(2)


print(l.data)