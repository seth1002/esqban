package jTASE;


import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;

import org.jfree.data.category.CategoryDataset;
import org.jfree.data.category.DefaultCategoryDataset;


public class Database
{
	private final String strConnection = "jdbc:mysql://192.168.2.106:3306/tase";
	private final String strUsername = "sqba";
	private final String strPassword = "crl2688";

	public CategoryDataset getCategoryDataset(String[] symbols) throws Exception
	{
		Class.forName("com.mysql.jdbc.Driver");
		
		Connection con = DriverManager.getConnection(strConnection, strUsername, strPassword);
		
		String strSymbols="";
		for(int i=0; i<symbols.length; i++)
		{
			if(i > 0)
				strSymbols += ", ";
			strSymbols += "'" + symbols[i] + "'";
		}
		
		PreparedStatement statement = con.prepareStatement("select date_, price, symbol, volume from all_prices where symbol in (" + strSymbols + ")");
		//"select date_, price, volume from all_prices where symbol='%s'"
		
		ResultSet result = statement.executeQuery();
		
		final DefaultCategoryDataset dataset = new DefaultCategoryDataset();
		while( result.next() )
		{
			//System.out.println(result.getString(1) + " " + result.getString(2) + " " + result.getString(3) + " " + result.getString(4));
			dataset.addValue(result.getDouble(2), result.getString(3), result.getString(1));
		}
		
		return dataset;
	}
}
