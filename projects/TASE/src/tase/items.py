# Define here the models for your scraped items
#
# See documentation in:
# http://doc.scrapy.org/topics/items.html

from scrapy.item import Item, Field

class NewsArticle(Item):
    date_ = Field()
    symbol = Field()
    url = Field()
    headline = Field()
    summary = Field()
    content = Field()

class MarketItem(Item):
    date_ = Field()
    name = Field()
    base_price = Field()
    opening_price = Field()
    closing_price = Field()
    high = Field()
    low = Field()
    market_cap = Field()
    symbols = Field()

class FinancialStatement(Item):
    period = Field()
    total_assets = Field()
    current_assets = Field()
    non_current_assets = Field()
    shareholders_equity = Field()
    of_which_minority_interest = Field()
    current_liabilities = Field()
    long_term_liabilities = Field()
    revenues = Field()
    gross_profit = Field()
    operating_income = Field()
    income_before_tax = Field()
    net_income = Field()
    income_atributable_to_shareholders = Field()
    earnings_per_share = Field()
    dividends = Field()
    net_cash_flow_generated_by_operating_activities = Field()
    market_to_book_value = Field()
    price_earning_ratio = Field()
    equity_assets_ratio = Field()
    return_on_equity = Field()

class TaseItem(Item):
    CompanyID = Field()
    ShareID = Field()

    image_urls = Field()
    images = Field()

    symbol = Field()
    name = Field()
    sector = Field()
    subsector = Field()
    capital = Field()
    marketcap = Field()
    url = Field()
    tase_url = Field()
    category = Field()
    pagerank = Field()
    
    financial_statements = Field()
    
    date_ = Field()
    adjusted_closing_price = Field()
    closing_price = Field()
    change_ = Field()
    gross_yield_to_maturity = Field()
    opening_price = Field()
    base_price = Field()
    high = Field()
    low = Field()
    capital_listed_for_trading = Field()
    market_cap = Field()
    turnover = Field()
    volume = Field()
    trans = Field()

    purchase_price = Field()
    redemption_price = Field()
    sales_load = Field()
    management_fee = Field()
    asset_value = Field()
    trustee_fee = Field()

class WeatherItem(Item):
    date_ = Field()
    temp_high = Field()
    temp_avg = Field()
    temp_low = Field()
    dew_high = Field()
    dew_avg = Field()
    dew_low = Field()
    hum_high = Field()
    hum_avg = Field()
    hum_low = Field()
    pres_high = Field()
    pres_avg = Field()
    pres_low = Field()
    vis_high = Field()
    vis_avg = Field()
    vis_low = Field()
    wind_high = Field()
    wind_avg = Field()
    wind_low = Field()
    precip = Field()
    events = Field()

def __str__(self):
    return "Company: symbol=%s url=%s" % (self['symbol'], self['url'])


