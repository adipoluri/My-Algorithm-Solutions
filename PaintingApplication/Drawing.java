//Made by Adi Poluri
//Interview Question for Castofly!
import java.awt.*;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.util.ArrayList;
import javax.swing.JFrame;
import javax.swing.JPanel;


public class Drawing extends JPanel {
    private ArrayList<ArrayList<Point>> toPaint;
    private MouseAdapter adapt;

    public Drawing() {
        toPaint = new ArrayList<>(400);
        createMouseAdapter();
        addMouseListener(adapt);
        addMouseMotionListener(adapt);
        this.setBackground(Color.BLACK);

    }

    //initialises and overides mouse methods
    private void createMouseAdapter() {

        adapt = new MouseAdapter() {
            private ArrayList<Point> currentPath;

            @Override
            public void mousePressed(MouseEvent e) {
                currentPath = new ArrayList<>(400);
                currentPath.add(e.getPoint());
                toPaint.add(currentPath);
            }

            @Override
            public void mouseDragged(MouseEvent e) {
                Point dragPoint = e.getPoint();
                currentPath.add(dragPoint);
                repaint();
            }

            @Override
            public void mouseReleased(MouseEvent e) {
                currentPath = null;
            }

        };
    }

    //iterates through array ToPaint and draws points
    //partially from JAVADocs on Java's Graphics2d library
    protected void paintComponent(Graphics g) {
        super.paintComponent(g);
        Graphics2D g2d = (Graphics2D) g.create();
        g2d.setColor(Color.WHITE);
        g2d.setStroke(new BasicStroke(4f));
        for (ArrayList<Point> path : toPaint) {
            Point start = null;
            for (Point end : path) {
                if (start != null) {
                    g2d.drawLine(start.x, start.y, end.x, end.y);
                }
                start = end;
            }
        }
        g2d.dispose();
    }

    //initialises the program
    public static void main(String[] args) {
        // Create the frame.

        JFrame frame = new JFrame("My Drawing for Interview");
        frame.add(new Drawing());
        frame.setSize(600, 600);
        frame.setVisible(true);
        frame.setResizable(false);
        frame.setLocationRelativeTo(null);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        System.out.println("Program Started!");
    }

}
