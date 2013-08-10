package jTASE;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;

import org.jfree.data.category.CategoryDataset;
import org.jfree.data.category.DefaultCategoryDataset;
import org.jfree.data.general.DefaultPieDataset;
import org.jfree.data.general.PieDataset;

public class Database
{
	private final String strConnection = "jdbc:mysql://192.168.2.106:3306/tase";
	private final String strUsername = "sqba";
	private final String strPassword = "crl2688";

	public CategoryDataset getCategoryDataset() throws Exception
	{
		Class.forName("com.mysql.jdbc.Driver");
		
		Connection con = DriverManager.getConnection(strConnection, strUsername, strPassword);
		
		PreparedStatement statement = con.prepareStatement("select date_, price, volume from all_prices where symbol='SNFL'");
		//"select date_, price, volume from all_prices where symbol='%s'"
		
		ResultSet result = statement.executeQuery();
		
		final DefaultCategoryDataset dataset = new DefaultCategoryDataset();
		while( result.next() )
		{
			//System.out.println(result.getString(1) + " " + result.getString(2) + " " + result.getString(3) + " " + result.getString(4));
			dataset.addValue(result.getDouble(2), "SNFL", result.getString(1));
		}
		
		return dataset;
	}
	
	public String getName()
	{
		return "SNFL";
	}
	
	public CategoryDataset createCategoryDataset()
	{
        // row keys...
        final String series1 = "First";
        final String series2 = "Second";
        final String series3 = "Third";

        // column keys...
        final String type1 = "Type 1";
        final String type2 = "Type 2";
        final String type3 = "Type 3";
        final String type4 = "Type 4";
        final String type5 = "Type 5";
        final String type6 = "Type 6";
        final String type7 = "Type 7";
        final String type8 = "Type 8";

        // create the dataset...
        final DefaultCategoryDataset dataset = new DefaultCategoryDataset();

        dataset.addValue(1.0, series1, type1);
        dataset.addValue(4.0, series1, type2);
        dataset.addValue(3.0, series1, type3);
        dataset.addValue(5.0, series1, type4);
        dataset.addValue(5.0, series1, type5);
        dataset.addValue(7.0, series1, type6);
        dataset.addValue(7.0, series1, type7);
        dataset.addValue(8.0, series1, type8);

        dataset.addValue(5.0, series2, type1);
        dataset.addValue(7.0, series2, type2);
        dataset.addValue(6.0, series2, type3);
        dataset.addValue(8.0, series2, type4);
        dataset.addValue(4.0, series2, type5);
        dataset.addValue(4.0, series2, type6);
        dataset.addValue(2.0, series2, type7);
        dataset.addValue(1.0, series2, type8);

        dataset.addValue(4.0, series3, type1);
        dataset.addValue(3.0, series3, type2);
        dataset.addValue(2.0, series3, type3);
        dataset.addValue(3.0, series3, type4);
        dataset.addValue(6.0, series3, type5);
        dataset.addValue(3.0, series3, type6);
        dataset.addValue(4.0, series3, type7);
        dataset.addValue(3.0, series3, type8);

        return dataset;
	}

	   public  PieDataset createPieDataset() {
	        DefaultPieDataset result = new DefaultPieDataset();
	        result.setValue("Linux", 29);
	        result.setValue("Mac", 20);
	        result.setValue("Windows", 51);
	        return result;
	        
	    }
}
