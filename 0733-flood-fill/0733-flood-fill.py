class Solution(object):
    def floodFill(self, image, sr, sc, color):
        """
        :type image: List[List[int]]
        :type sr: int
        :type sc: int
        :type color: int
        :rtype: List[List[int]]
        """

        dfs = [[sr,sc]]
        orig = image[sr][sc]
        retImg = image

        # Edge Cases:
        if color == orig:
            return retImg

        # validate that sr sc in bounds
        if sr > len(image)-1 or sr < 0 or sc > len(image)-1 or sc < 0:
            return retImg

        while len(dfs) != 0:
            x,y = dfs.pop(0)

            if x > len(image)-1 or x < 0 or y > len(image[0])-1 or y < 0:
                continue

            if retImg[x][y] == orig:
                retImg[x][y] = color

                dfs.append([x+1,y])
                dfs.append([x-1,y])
                dfs.append([x,y+1])
                dfs.append([x,y-1])
                
            
        return retImg


