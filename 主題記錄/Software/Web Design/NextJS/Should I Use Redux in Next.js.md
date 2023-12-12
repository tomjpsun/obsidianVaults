來源：
https://javascript.plainenglish.io/should-you-use-redux-in-next-js-5e57201c34da

**答案是：不是一定不要，只是沒有那麼必要 

## 場景 1：您已經知道頁面上的內容

如果像這樣：後續的訪問頁面，都已經知道了：

```
/products -> 顯示產品列表
/product/{productId} -> 顯示產品的詳細信息
```

就可以取得到：

```js
export async function getStaticProps(context: GetStaticPropsContext) {
    
    const products = await getProducts();
    
    return {
        props: {
            products
        }
    };
}
```

如果產品列表隨著時間的推移發生變化，您可以使用getServerSideProps此函數來代替：

```js
export async function getServerSideProps(context: GetServerSidePropsContext) {
    const request = context.req;
   
    const products: getProductList()
  
    return { 
      props: {
        products
      }
    }
}
```

## 場景 2：你不知道會發生什麼
就像亞馬遜網站上的以下分頁：

![[Pasted image 20230906142559.png]]

用 swr 或 react-query 即可做到，以下是使用react-query的示例：

```js
const [page, setPage] = React.useState(0)

const fetchProducts = (page = 0) => fetch('/api/products?page=' + page).then((res) => res.json())

const { data} = useQuery(['products', page], () => fetchProducts(page), { keepPreviousData : true })
```

## 場景 3: 你需要跨 Components 分享狀態

使用 browser 提供的 LocalStorage 就可以搞定

```js
const useAuthState = () => {
  
  const setAuthState = ( currentState ) => {
    localStorage.set("auth_state" , currentState)
  }
  
  const getAuthState = () => {
    return localStorage.get("auth_state" ,false);
  }
  
  return { setAuthState , getAuthState }
}
```

如果 LocalStorage 不夠用，那也是優先考慮 React Context API ，因爲是 React 自帶的，不需要額外的安裝成本


