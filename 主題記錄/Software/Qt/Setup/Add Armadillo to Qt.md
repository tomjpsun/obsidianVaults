Armadillo 是矩陣運算的 library，它也涵蓋統計與三角函數的部分。
使用時先安裝：
```bash
sudo apt install libarmadillo-dev
```

要在 qmake 裏面使用，需要加上 INCLUDEPATH 與 LIBS：

```qmake
INCLUDEPATH += /usr/include/armadillo_bits
LIBS += -larmadillo
```

使用時 include  *\<armadillo\>*

```c++
#include <armadillo>

using namespace std;
using namespace arma;

void test()
{
    mat A(5,5,fill::randu);
    mat B(5,5,fill::randu);
    mat C = A*B.t();
    C.print("C = A*B.t()");
}
```

output:
```
C = A*B.t()
   0.9316   1.5874   1.4487   1.7611   0.5737
   0.7121   1.0869   0.9806   1.0323   0.4157
   0.5629   1.7454   1.3832   1.2484   0.4273
   1.0664   1.6669   1.7805   1.8962   0.6785
   0.5541   0.6851   0.7394   0.6493   0.4493
```
#### finish
