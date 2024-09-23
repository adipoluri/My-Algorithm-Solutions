class Solution(object):
    def romanToInt(self, s):
        """
        :type s: str
        :rtype: int
        """
        count = 0
        prev = None
        for symbol in s:
            
            if symbol == "I":
                count += 1

            elif symbol == "V":
                if prev == "I":
                    count += 3
                else:
                    count += 5

            elif symbol == "X":
                if prev == "I":
                    count += 8
                else:
                    count += 10

            elif symbol == "L":
                if prev == "X":
                    count += 30
                else:
                    count += 50

            elif symbol == "C":
                if prev == "X":
                    count += 80
                else:
                    count += 100

            elif symbol == "D":
                if prev == "C":
                    count += 300
                else:
                    count += 500

            elif symbol == "M":
                if prev == "C":
                    count += 800
                else:
                    count += 1000

            prev = symbol
        return count
