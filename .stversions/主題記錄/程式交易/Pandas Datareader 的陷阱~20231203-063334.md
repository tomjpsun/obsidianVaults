[0050 K 線圖](https://github.com/jumpingchu/Stock-Chart/blob/master/0050%20K%E7%B7%9A%E5%9C%96.py)
TypeError: string indices must be integers, not 'str'

正確的用 yfinance override

```python
import pandas
from pandas_datareader import data as pdr
import yfinance as yfin


yfin.pdr_override()

spy = pdr.get_data_yahoo('0050.TW', start='2022-10-24', end='2022-12-23')

print(spy)
```


