

- Post author:[Ben](https://www.finlab.tw/author/ben/ "「Ben」的文章")
- Post published:2020-07-20
- Reading time:10 mins read

Tags: [BOKEH](https://www.finlab.tw/tag/bokeh/), [PYTHON](https://www.finlab.tw/tag/python/), [資料視覺化](https://www.finlab.tw/tag/%e8%b3%87%e6%96%99%e8%a6%96%e8%a6%ba%e5%8c%96/)

**內容目錄** [隱藏](https://www.finlab.tw/bokeh-stock-chart-with-technical-analysis/#)

[1 改造動機](https://www.finlab.tw/bokeh-stock-chart-with-technical-analysis/#gai_zao_dong_ji)

[1.1 github:https://github.com/benbilly3/bokeh_explore](https://www.finlab.tw/bokeh-stock-chart-with-technical-analysis/#githubhttpsgithubcombenbilly3bokeh_explore)

[2 繪圖技巧說明](https://www.finlab.tw/bokeh-stock-chart-with-technical-analysis/#hui_tu_ji_qiao_shuo_ming)

[2.1 1. figure圖紙設定，bokeh各種models應用](https://www.finlab.tw/bokeh-stock-chart-with-technical-analysis/#1_figure_tu_zhi_she_dingbokeh_ge_zhongmodels_ying_yong)

[2.2 2. 處理假日日期造成的資料不連續問題，x_range overwrite技巧](https://www.finlab.tw/bokeh-stock-chart-with-technical-analysis/#2_chu_li_jia_ri_ri_qi_zao_cheng_de_zi_liao_bu_lian_xu_wen_tix_range_overwrite_ji_qiao)

[2.3 3. hover互動資料顯示](https://www.finlab.tw/bokeh-stock-chart-with-technical-analysis/#3_hover_hu_dong_zi_liao_xian_shi)

[2.4 4. legend物件控制，從label控制線圖開關。將legend移到圖表外，讓版面清晰。](https://www.finlab.tw/bokeh-stock-chart-with-technical-analysis/#4_legend_wu_jian_kong_zhi_conglabel_kong_zhi_xian_tu_kai_guanjianglegend_yi_dao_tu_biao_wai_rang_ban_mian_qing_xi)

[2.5 5. 位移、縮放、十字線、重置、存檔工具](https://www.finlab.tw/bokeh-stock-chart-with-technical-analysis/#5_wei_yi_suo_fang_shi_zi_xian_zhong_zhi_cun_dang_gong_ju)

[2.6 6. second y_ranges繪製(使用雙軸)](https://www.finlab.tw/bokeh-stock-chart-with-technical-analysis/#6_second_y_ranges_hui_zhi_shi_yong_shuang_zhou)

[2.7 7. 利用vbar和segment快速繪製k線,並將均線帶入。](https://www.finlab.tw/bokeh-stock-chart-with-technical-analysis/#7_li_yongvbar_hesegment_kuai_su_hui_zhik_xian_bing_jiang_jun_xian_dai_ru)

[2.8 8. 建立技術線子圖組](https://www.finlab.tw/bokeh-stock-chart-with-technical-analysis/#8_jian_li_ji_shu_xian_zi_tu_zu)

[2.8.1 經過神奇的魔法就有hahow課程的進化版，包含以上的功能，實踐了不錯的效果！加入output_file(‘檔名’)就可產生html檔在瀏覽器使用，只要再寫一個傳導查詢變數的API，就能串接做一個服務出來，之後會講用FastApi寫後端來串Bokeh。](https://www.finlab.tw/bokeh-stock-chart-with-technical-analysis/#jing_guo_shen_qi_de_mo_fa_jiu_youhahow_ke_cheng_de_jin_hua_ban_bao_han_yi_shang_de_gong_neng_shi_jian_le_bu_cuo_de_xiao_guo_jia_ruoutput_file_dang_ming_jiu_ke_chan_shenghtml_dang_zai_liu_lan_qi_shi_yong_zhi_yao_zai_xie_yi_ge_chuan_dao_cha_xun_bian_shu_deAPI_jiu_neng_chuan_jie_zuo_yi_ge_fu_wu_chu_lai_zhi_hou_hui_jiang_yongFastApi_xie_hou_duan_lai_chuanBokeh)

[3 課程相容資料提取工具](https://www.finlab.tw/bokeh-stock-chart-with-technical-analysis/#ke_cheng_xiang_rong_zi_liao_ti_qu_gong_ju)

[3.1 從python小資族的sqlite提取資料](https://www.finlab.tw/bokeh-stock-chart-with-technical-analysis/#congpython_xiao_zi_zu_desqlite_ti_qu_zi_liao)

[4 程式碼下載](https://www.finlab.tw/bokeh-stock-chart-with-technical-analysis/#cheng_shi_ma_xia_zai)

[4.1 github:https://github.com/benbilly3/bokeh_explore](https://www.finlab.tw/bokeh-stock-chart-with-technical-analysis/#githubhttpsgithubcombenbilly3bokeh_explore1)

### 改造動機

[上次介紹了Bokeh厲害的地方](https://www.finlab.tw/python-bokeh1-setup-and-first-impression/)，那可不可以應用到投資圖表的繪製呢？
在Hahow課程：『Python 理財：打造小資族選股策略』的單元20中，韓老師用Talib和matplotlib套件示範了如何提取資料和客製化技術圖表繪圖，點出應用方向，然而課程重點是放在選股，不是在視覺化，圖表功能比較簡單，當時Bokeh也沒那麼厲害。

![Q8hEt30 1](http://34.96.136.135/wp-content/uploads/2020/07/Q8hEt30-1-1024x557.png "Bokeh 探索頻道(2)~客製化技術圖表升級 1")

有些同學希望能有更精美的圖表可使用，搭配課程精彩的選股程式技巧，不就更完美了？這個願望今天就可以實現，後面有銜接課程的程式碼直接提供使用。
現在有Bokeh的幫忙，用Python也能打造不錯的互動圖表效果，接下來就要綜合小資族課程、Bokeh、網路上各路技巧，來打造適用於Hahow課程的新圖表。沒參與課程的可以用github 連結裡的demo_json檔來試試，只要資料格式對應，都可使用。

#### github:[https://github.com/benbilly3/bokeh_explore](https://github.com/benbilly3/bokeh_explore)

使用的程式檔為technical_chart。

### 繪圖技巧說明

寫Python就要利用其他魔法師的咒語，省時有效率，先來Bokeh gallery看看沒有範例。
結果有現成的Ｋ線圖範例，太好了！

![wjTomBS](https://i.imgur.com/wjTomBS.png "Bokeh 探索頻道(2)~客製化技術圖表升級 2")

雖然仍是陽春的靜態圖，但至少有了改造藍圖，從官網的程式碼：
[https://docs.bokeh.org/en/latest/docs/gallery/candlestick.html](https://docs.bokeh.org/en/latest/docs/gallery/candlestick.html)

可以發現bokeh輕鬆地用pandas分類紅黑棒資料，再用segment畫引線和用vbar畫長棒圖。眼尖的人可以發現股價碰到假日的時候會有空值，這造成閱讀上有些礙眼，能否有讓時間序列日期資料的解決辦法呢？

在還不熟bokeh的時候，stackoverflow也是我們好朋友．．．處理關鍵在讓日期轉為文字，不用會自動補假日日期的datetime。

```python
fig.xaxis.major_label_overrides = {
            i: date.strftime('%Y/%m/%d') for i, date in enumerate(pd.to_datetime(df["date"]))
            # pd.date_range(start='3/1/2000', end='1/08/2018')
        }
```

bokeh是物件導向繪圖庫，都封裝相當好，基本上沒啥程式技巧，就像玩樂高積木一樣，搜尋工具來堆，不難，比較繁瑣，內容頗多。
接著主要會實踐下列功能到圖表，有興趣學的可以看連結。

#### 1. figure圖紙設定，bokeh各種models應用

[https://docs.bokeh.org/en/latest/docs/reference/plotting.html](https://docs.bokeh.org/en/latest/docs/reference/plotting.html)

#### 2. 處理假日日期造成的資料不連續問題，x_range overwrite技巧

[https://stackoverflow.com/questions/23585545/how-do-i-make-bokeh-omit-missing-dates-when-using-datetime-as-x-axis](https://stackoverflow.com/questions/23585545/how-do-i-make-bokeh-omit-missing-dates-when-using-datetime-as-x-axis)

#### 3. hover互動資料顯示

[https://docs.bokeh.org/en/latest/docs/user_guide/tools.html](https://docs.bokeh.org/en/latest/docs/user_guide/tools.html)

#### 4. legend物件控制，從label控制線圖開關。將legend移到圖表外，讓版面清晰。

[https://stackoverflow.com/questions/26254619/position-of-the-legend-in-a-bokeh-plothttps://docs.bokeh.org/en/latest/docs/user_guide/interaction/legends.html](https://stackoverflow.com/questions/26254619/position-of-the-legend-in-a-bokeh-plothttps://docs.bokeh.org/en/latest/docs/user_guide/interaction/legends.html)

#### 5. 位移、縮放、十字線、重置、存檔工具

[https://docs.bokeh.org/en/latest/docs/user_guide/tools.html](https://docs.bokeh.org/en/latest/docs/user_guide/tools.html)

#### 6. second y_ranges繪製(使用雙軸)

[https://docs.bokeh.org/en/latest/docs/user_guide/plotting.html#userguide-plotting-twin-axes](https://docs.bokeh.org/en/latest/docs/user_guide/plotting.html#userguide-plotting-twin-axes)

#### 7. 利用vbar和segment快速繪製k線,並將均線帶入。

[https://docs.bokeh.org/en/latest/docs/gallery/candlestick.html](https://docs.bokeh.org/en/latest/docs/gallery/candlestick.html)

#### 8. 建立技術線子圖組

[https://docs.bokeh.org/en/latest/docs/user_guide/layout.html#userguide-layout](https://docs.bokeh.org/en/latest/docs/user_guide/layout.html#userguide-layout)

##### 經過神奇的魔法就有hahow課程的進化版，包含以上的功能，實踐了不錯的效果！加入output_file(‘檔名’)就可產生html檔在瀏覽器使用，只要再寫一個傳導查詢變數的API，就能串接做一個服務出來，之後會講用FastApi寫後端來串Bokeh。

![4BY8lBt](https://i.imgur.com/4BY8lBt.png "Bokeh 探索頻道(2)~客製化技術圖表升級 3")

### 課程相容資料提取工具

#### 從python小資族的sqlite提取資料

若DB為pickle檔，須將pd.read_sql那行修改為read_pickle，並要注意格式。

如果有上過韓老師python小資族，可將course11.ipynb上頭用read_sql取資料的程式改為下部份取資料，輸入股號和資料開始日期，產出的data丟入後面的繪圖程式，即可產生互動式圖表。

```python
import pandas as pd
import sqlite3
import os
import json
from talib import abstract


def get_price_data(stock_id,date):
    # connect to sql
    conn = sqlite3.connect(os.path.join('data', "data.db"))
    # read data from sql
    df = pd.read_sql(f"select stock_id,證券名稱, date, 開盤價, 收盤價, 最高價, 最低價, 成交股數 from price where stock_id='{stock_id}' and date > '{date}'", conn,
        index_col=['date'])
    # rename the columns of dataframe
    df.index=df.index.astype(str).str.split(" ").str[0]
    df.rename(columns={'收盤價':'close','證券名稱':'name', '開盤價':'open', '最高價':'high', '最低價':'low', '成交股數':'volume'}, inplace=True)
    df['MA5']=df['close'].rolling(5).mean()
    df['MA10']=df['close'].rolling(10).mean()
    df['MA20']=df['close'].rolling(20).mean()
    df['MA60']=df['close'].rolling(60).mean()
    df['MA120']=df['close'].rolling(120).mean()
    df['volume']=df['volume']/1000


    RSI = pd.DataFrame(abstract.RSI(df, timeperiod=12),columns=['RSI_12'])
    RSI['RSI_36']=abstract.RSI(df, timeperiod=36)
    RSI=RSI.to_dict()
    STOCH = abstract.STOCH(df).to_dict()
    MACD=abstract.MACD(df).to_dict()
    basic=df.iloc[-1,:2].to_dict()
    df=df.drop(columns=['stock_id','name']).to_dict()
    data={'basic':basic,'price_df':df,'RSI':RSI,'STOCH' :STOCH,'MACD':MACD }

    return data
```

### 程式碼下載

使用的繪圖程式檔為technical_chart，將get_price_data(stock_id,date)帶入technical_chart(json_df)即可繪圖，可到gitlab下載bokeh_explore。

#### github:[https://github.com/benbilly3/bokeh_explore](https://github.com/benbilly3/bokeh_explore)

```python
import pandas as pd
from bokeh.io import show, output_file, output_notebook
from bokeh.plotting import figure
from bokeh.models import SingleIntervalTicker, LinearAxis, ColumnDataSource, HoverTool
from bokeh.palettes import Spectral4, Dark2
from bokeh.models import LinearAxis, Range1d, Legend
from bokeh.layouts import column


def technical_chart(json_df):
    # get data
    stock_data = json_df
    df = pd.DataFrame(stock_data['price_df']).reset_index().rename(columns={'index': 'date'})
    df = df.reset_index()
    inc = df.close > df.open
    dec = df.open > df.close
    inc_data = df[inc]
    dec_data = df[dec]
    df_source = ColumnDataSource(df)
    inc_source = ColumnDataSource(inc_data)
    dec_source = ColumnDataSource(dec_data)

    # set hover
    hover = HoverTool(
        tooltips=[
            ("date", "@date"),
            ("close", "@open"),
            ("open", "@close"),
            ("high", "@high"),
            ("low", "@low"),
            ("volume", "@volume")
        ],
        formatters={"@date": "datetime"}
    )

    hover_rsi_kd = HoverTool(
        tooltips=[
            ("date", "@date"),
            ("RSI_12", "@RSI_12"),
            ("RSI_36", "@RSI_36"),
            ("slowk", "@slowk"),
            ("slowd", "@slowd"),

        ],
        formatters={"@date": "datetime"}

    )

    hover_macd = HoverTool(
        tooltips=[
            ("date", "@date"),
            ("macd", "@macd"),
            ("macdsignal", "@macdsignal"),
            ("macdhist", "@macdhist"),
        ],
        formatters={"@date": "datetime"}

    )

    # set figure data
    basic_data = stock_data['basic']
    title = basic_data['stock_id'] + ' ' + basic_data['name'] + ' ' + 'technical_chart'
    x_end = len(df)
    show_init_num = 120
    x_start = x_end - show_init_num
    interval_freq = show_init_num / 12
    y_start = df['close'].min() * 0.95
    y_end = df['close'].max() * 1.05
    p1 = figure(plot_width=1000, title=title, plot_height=500, x_range=(x_start, x_end), y_range=(y_start, y_end),
                tools=[hover, "pan,zoom_in,zoom_out,crosshair,reset,save"], toolbar_location="above" ,y_axis_label="price")
    p2 = figure(plot_width=1000, title='RSI&KD', plot_height=250, x_range=(x_start, x_end),
                background_fill_color="#fafafa", tools=[hover_rsi_kd, "pan,zoom_in,zoom_out,crosshair,reset,save"],
                toolbar_location="above")
    p3 = figure(plot_width=1000, title='MACD', plot_height=250, x_range=(x_start, x_end),
                background_fill_color="#fafafa", tools=[hover_macd, "pan,zoom_in,zoom_out,crosshair,reset,save"],
                toolbar_location="above")

    for fig in [p1, p2, p3]:
        fig.title.text_font_size = '16pt'

        # map dataframe indices to date strings and use as label overrides
        fig.xaxis.major_label_overrides = {
            i: date.strftime('%Y/%m/%d') for i, date in enumerate(pd.to_datetime(df["date"]))
            # pd.date_range(start='3/1/2000', end='1/08/2018')
        }
        fig.xaxis.ticker = SingleIntervalTicker(interval=interval_freq)

    # set k bar chart
    # use the *indices* for x-axis coordinates, overrides will print better labels
    p1.segment('index', 'high', 'index', 'low', color="black", source=df_source)
    p1.vbar('index', 0.5, 'open', 'close', fill_color="#eb2409", line_color="black", source=inc_source)
    p1.vbar('index', 0.5, 'open', 'close', fill_color="#00995c", line_color="black", source=dec_source)

    # set ma line
    ma_legend_items = []
    for ma_name, color in zip(["MA5", "MA10", "MA20", "MA60", "MA120"], Dark2[5]):
        ma_df = df[['index', 'date', 'close', 'open', 'high', 'low', 'volume', ma_name]]
        source = ColumnDataSource(ma_df)
        ma_line = p1.line(x="index", y=ma_name, line_width=2, color=color, alpha=0.8,
                          muted_color=color, muted_alpha=0.2, source=source)
        ma_legend_items.append((ma_name, [ma_line]))

    # set ma legend
    legend = Legend(items=ma_legend_items, location=(0, 250))
    p1.add_layout(legend, 'left')

    # set twinx for volume
    y2_start = df['volume'].min() * 0.95
    y2_end = df['volume'].max() * 2
    p1.extra_y_ranges = {"vol": Range1d(y2_start, y2_end)}
    p1.vbar('index', 0.5, 'volume', y_range_name='vol', color='blue', alpha=0.2, source=df_source)
    p1.add_layout(LinearAxis(y_range_name="vol" ,axis_label="vol"), 'right')

    # RSI Chart
    rsi_df = pd.DataFrame(stock_data['RSI']).reset_index().rename(columns={'index': 'date'})
    kd_df = pd.DataFrame(stock_data['STOCH']).reset_index().rename(columns={'index': 'date'})
    rsi_kd_df = pd.concat([rsi_df, kd_df], axis=1)
    rsi_source = ColumnDataSource(rsi_kd_df)

    rsi_kd_legend_items = []
    for index_name, color in zip(["RSI_12", "RSI_36", "slowk", "slowd"], Spectral4):
        index_line = p2.line('index', index_name, line_width=3, color=color, alpha=0.8, muted_color=color,
                             muted_alpha=0.2, source=rsi_source)

        rsi_kd_legend_items.append((index_name, [index_line]))

    # set rsi_kd legend
    legend = Legend(items=rsi_kd_legend_items, location=(0, 50))
    p2.add_layout(legend, 'left')

    # MACD Chart
    macd = pd.DataFrame(stock_data['MACD']).reset_index().rename(columns={'index': 'date'})
    macd = macd.reset_index()
    macd_source = ColumnDataSource(macd)

    macd_legend_items = []
    for index_name, color in zip(["macd", "macdsignal", "macdhist"], Dark2[3]):
        if index_name == "macdhist":
            index_line = p3.vbar('index', 0.5, index_name, color=color, alpha=0.8, muted_color=color, muted_alpha=0.2,
                                 source=macd_source)
        else:
            index_line = p3.line('index', index_name, line_width=3, color=color, alpha=0.8, muted_color=color,
                                 muted_alpha=0.2, source=macd_source)

        macd_legend_items.append((index_name, [index_line]))

    # set macd legend
    legend = Legend(items=macd_legend_items, location=(0, 50))
    p3.add_layout(legend, 'left')

    # set legend mode
    for fig in [p1, p2, p3]:
        # set legend
        fig.legend.label_text_font_size = '8pt'
        # use hide or mute
        fig.legend.click_policy = "hide"
        #     fig.add_layout(legend, 'left')

    # use brower output
#     output_file("candlestick.html", title="candlestick.py example")
    # use jupyter output
    output_notebook()
    # use columns to control all fig  locations,you could try row method.
    show(column(p1, p2, p3))
```