發生這個 error 的現象：

```js
const Home: NextPage<IConfigure> = (props: IConfigure) => {
  localStorage.setItem("configureData", JSON.stringify(props));
  return <Dashboard />;
};
export default Home;
```

[參考網頁](https://developer.school/snippets/react/localstorage-is-not-defined-nextjs)

因爲 NexgPage 在 server render 的階段無法取得 browser window, 而 localStorage 正是寫入 browser window 的 API ，所以，要改爲 *有 window 時才呼叫* , 也就是等到 CSR(Client Side Rendering) 才執行:

```js
const Home: NextPage<IConfigure> = (props: IConfigure) => {
  useEffect(() => {
    localStorage.setItem("configureData", JSON.stringify(props));
  });
  return <Dashboard />;
};

export default Home;
```

關於 compile 時，什麼情況會將 Page 留到 SSR ？
[這裏列兩種狀況](https://nextjs.org/docs/pages/building-your-application/rendering/client-side-rendering)
1 放在 useEffect() 裏面
2 放在 swr Library 裏面
