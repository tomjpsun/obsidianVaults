# 發生情形

在 Home 頁面，採用 Recoil 記錄環境給的參數，例如 backend URL/Port，可能會看到這個警告： 

Warning: Cannot update a component (`Batcher`) while rendering a different component (`Home`). To locate the bad setState() call inside `Home`, follow the stack trace as described in [https://reactjs.org/link/setstate-in-render](https://reactjs.org/link/setstate-in-render "https://reactjs.org/link/setstate-in-render")	

這是 React + Recoil 才可能看到。網絡有人也遇過：

https://stackoverflow.com/questions/62336340/cannot-update-a-component-while-rendering-a-different-component-warning

出現這個狀況的地方如下：

```ts
const Home: NextPage<HomeProps> = (props) => {
  const [backendAddr, setBackendAddr] = useRecoilState(BackendAddr);
  const [backendPort, setBackendPort] = useRecoilState(BackendPort);
  MyLogger("backendAddr:" + backendAddr);
  MyLogger("backendPort:" + backendPort);
  MyLogger("prop:" + `${props.backend_url} ${props.backend_port}`);

  setBackendAddr(props.backend_url);
  setBackendPort(props.backend_port);

  return <Content />;
};
```

把 Recoil set state 包在 useEffect 裏面就可解決這個warning,像這樣：

```ts
...
useEffect(() => {
	setBackendAddr(props.backend_url);
	setBackendPort(props.backend_port);
});

```