import java.util.Arrays;

public class MagicSum {
    public int[][] map;
    private final int[] nums;

    private int[] pos;
    private int[] tempPos;

    private final int width;
    public MagicSum(int rowSize,int firstNum) {
        map = new int[rowSize][rowSize];
        nums = new int[rowSize*rowSize];

        int counter = firstNum;
        for(int i = 0; i < rowSize*rowSize; i++) {
            nums[i] = counter;
            counter++;
        }

        width = rowSize;
        pos = new int[2];
        tempPos = new int[2];

    }

    public void solveMap() {
        //Initializes the starting point
        pos[1] = (width - 1)/2;
        map[pos[0]][pos[1]] = nums[0];

        for(int i = 1; i < nums.length; i++) {
            handleMove();
            map[pos[0]][pos[1]] = nums[i];
        }
    }


    private void handleMove() {
        tempPos = pos.clone();

        tempPos[0]--;
        tempPos[1]++;

        if(CheckOutOfBounds()) {
            MoveIfOutOfBounds();
        }

        if(OnExistingNumber()) {
            tempPos = pos.clone();
            tempPos[0]++;
            MoveIfOutOfBounds();
        }
        pos = tempPos.clone();
    }


    private boolean CheckOutOfBounds() {
        if(tempPos[0] >= width || tempPos[0] < 0) {
            return true;
        }
        return tempPos[1] >= width || tempPos[1] < 0;
    }

    private boolean OnExistingNumber() {
        return map[tempPos[0]][tempPos[1]] > 0;
    }

    private void MoveIfOutOfBounds() {
        if(tempPos[0] < 0) {
            tempPos[0] = width - 1;
        }
        if(tempPos[0] >= width) {
            tempPos[0] = 0;
        }
        if(tempPos[1] < 0) {
            tempPos[1] = width - 1;
        }
        if(tempPos[1] >= width) {
            tempPos[1] = 0;
        }
    }

    public void printMagicSum() {
        for(int i = 0; i < width; i++) {
            System.out.println(Arrays.toString(map[i]));
        }
    }

}
