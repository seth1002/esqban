package jTASE;

import org.jfree.data.general.PieDataset;
import org.jfree.data.category.CategoryDataset;

public class Main
{

	
	private static void test_PieChart() throws Exception
	{
		Database db = new Database();
		PieDataset dataSet = db.createPieDataset();
        Chart demo = new Chart("Comparison", "Which operating system are you using?", dataSet);
        demo.pack();
        demo.setVisible(true);		
	}
	
	private static void test_CategoryChart() throws Exception
	{
		Database db = new Database();
		CategoryDataset dataSet = db.getCategoryDataset();
		String name = db.getName();
        Chart demo = new Chart("Comparison", name, dataSet);
        demo.pack();
        demo.setVisible(true);		
	}

	public static void main(String[] args) throws Exception
	{
		//test_PieChart();
		test_CategoryChart();
	}
}
