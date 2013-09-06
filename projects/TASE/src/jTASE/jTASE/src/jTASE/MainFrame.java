package jTASE;

import javax.swing.JFrame;
import javax.swing.JSplitPane;
import javax.swing.JList;
import javax.swing.ListSelectionModel;
import javax.swing.JScrollPane;
import java.awt.Dimension;

import org.jfree.data.category.CategoryDataset;

public class MainFrame extends JFrame
{
	  private static final long serialVersionUID = 2L;
	  
	  public MainFrame()
	  {
		  super("TASE");
	  }


		public static void main(String[] args) throws Exception
		{
			Database db = new Database();
			String[] symbols = { "SNFL", "SALO-M", "ORBI", /*"TEVA", "DLEKG"*/ };
			CategoryDataset dataSet = db.getCategoryDataset(symbols);
	        Chart demo = new Chart("Comparison", "Price", dataSet);
	        
	        
	        
	        JList list = new JList();
	        list.setSelectionMode(ListSelectionModel.SINGLE_INTERVAL_SELECTION);
	        list.setLayoutOrientation(JList.HORIZONTAL_WRAP);
	        list.setVisibleRowCount(-1);
	        JScrollPane listScroller = new JScrollPane(list);
	        listScroller.setPreferredSize(new Dimension(250, 80));
	        
	        
	        
	        JSplitPane splitPane = new JSplitPane(JSplitPane.HORIZONTAL_SPLIT, listScroller, demo.getPanel());
	        
	        
	        
	        MainFrame mainFrame = new MainFrame();
	        mainFrame.setContentPane( splitPane );
	        mainFrame.pack();
	        mainFrame.setVisible(true);		
		}
}
