package jTASE;


import org.jfree.data.category.CategoryDataset;


public class Main
{
	public static void main(String[] args) throws Exception
	{
		Database db = new Database();
		String[] symbols = { "SNFL", "SALO-M", "ORBI", /*"TEVA", "DLEKG"*/ };
		CategoryDataset dataSet = db.getCategoryDataset(symbols);
        Chart demo = new Chart("Comparison", "Price", dataSet);
        demo.pack();
        demo.setVisible(true);		
	}
}
