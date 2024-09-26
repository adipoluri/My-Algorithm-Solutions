class Solution(object):
    def updateMatrix(self, mat):
        """
        :type mat: List[List[int]]
        :rtype: List[List[int]]
        """
        res = mat
        bfs = []
        for n in range(len(res)):
            for m in range(len(res[0])):
                if res[n][m] == 0:
                    bfs.append([n,m,1])
                else:
                    res[n][m] = -1

        directions = [[-1,0],[1,0],[0,-1],[0,1]]
        while len(bfs) > 0:
            n,m,dist = bfs.pop(0)
            for dx, dy in directions:
                next_row, next_col = n + dy, m + dx
                
                if next_row < 0 or next_row >= len(mat) or next_col < 0 or next_col >= len(mat[0]):
                    continue

                if res[next_row][next_col] == -1:
                    res[next_row][next_col] = dist
                    bfs.append([next_row,next_col,dist+1])
                
        return mat


    def sol1(self,matInput):
        mat = matInput
        bfs = []
        for n in range(len(mat)):
            for m in range(len(mat[0])):
                if mat[n][m] == 0:
                    bfs.append([n-1,m,1])
                    bfs.append([n+1,m,1])
                    bfs.append([n,m-1,1])
                    bfs.append([n,m+1,1])
                else:
                    mat[n][m] = -1

        while len(bfs) > 0:
            n,m,dist = bfs.pop(0)
            if n < 0 or n >= len(mat) or m < 0 or m >= len(mat[0]):
                continue

            if mat[n][m] == -1:
                mat[n][m] = dist
                bfs.append([n-1,m,dist+1])
                bfs.append([n+1,m,dist+1])
                bfs.append([n,m-1,dist+1])
                bfs.append([n,m+1,dist+1])
                
        return mat