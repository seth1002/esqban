SELECT base.date_, 
                base.symbol, 
                ((base.closing_price - old.closing_price) / old.closing_price) as gain 
           FROM company_prices as base
LEFT OUTER JOIN company_prices as old
             ON base.symbol = old.symbol
            AND base.date_ = DATE_ADD(old.date_, INTERVAL 45 DAY);
			