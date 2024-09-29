class MinStack(object):

    def __init__(self):
        self.stack = []
        self.min = (2**(31))-1

    def push(self, val):
        """
        :type val: int
        :rtype: None
        """
        if val <= self.min:
            self.min = val
        self.stack.append([val, self.min])
        

    def pop(self):
        """
        :rtype: None
        """
        val = self.stack.pop()
        if len(self.stack) == 0:
            self.min = (2**(31))-1
        else:
            self.min = self.getMin()


    def top(self):
        """
        :rtype: int
        """
        return self.stack[-1][0]
        

    def getMin(self):
        """
        :rtype: int
        """
        return self.stack[-1][1]
        


# Your MinStack object will be instantiated and called as such:
# obj = MinStack()
# obj.push(val)
# obj.pop()
# param_3 = obj.top()
# param_4 = obj.getMin()