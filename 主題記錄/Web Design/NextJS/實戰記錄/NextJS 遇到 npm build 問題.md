現象是這樣的：我們希望 BackendUrl 這個 component, 不一定要綁在每一個 page 裏面才能使用，所以界面設計成這樣：
```js
/* data.json */
{ backendAddr: "http:/127.0.0.1",
  backendPort: "3000"
}
```
```js

import config from "../data.json"
enum ApiDef {
  GET_DEVICE_STATE = 0,
  SAMPLE_PARAMS = 1,
  TAG_SEARCH = 2,
  DEFAULT = 100,
}

const BackendUrl = (api: number, query: QueryObject = {}) => {
	const url = config.backendAddr + ":" + config.backendPort;
	...
}
```
QueryObject 是附帶的 {key_1, val_2, key_2:val_2,... } 的參數，在這裏不是重點。
重點是 url，直接讀 data.json 得到， default 值是 "http:127.0.0.1:3000"，
會在 app 初始頁面更改爲設定檔提供的參數，所以，呼叫 API 像這樣：
```js
const ReaderSettings = (props: { url: string }) => {
  const api = BackendUrl(apiDef.GET_DEVICE_STATE);
  ...
}
```
在 _npm run build_ 的時候，會出現這個 error message：
``` bash
...
TypeError: fetch failed
    at Object.fetch (node:internal/deps/undici/undici:11413:11)
    at process.processTicksAndRejections (node:internal/process/task_queues:95:5) {
  cause: Error: connect ECONNREFUSED 127.0.0.1:3000
      at TCPConnectWrap.afterConnect [as oncomplete] (node:net:1494:16) {
    errno: -111,
    code: 'ECONNREFUSED',
    syscall: 'connect',
    address: '127.0.0.1',
    port: 3000
  }
}
...
```
這個 Error 可能是 ReaderSettings 這個頁面，在 build 的時候把 BackendUrl 優化爲 SSR(Server Side Rendering), 因此展開 config 的值得到 default (127.0.0.1), 而不是我們正確的 IP （這裏是
192.168.249.112）
修改方法就是 BackendUrl 的 URL 由每個頁面提供，在每一頁的 getStaticProps() 讀進來, 透過 props 送給 BackendUrl：
```js
export async function getStaticProps() {
  const url: string = process.env.BACKEND_URL + ":" + process.env.BACKEND_PORT;
  return {
    props: {
      url,
    },
  };
}

const ReaderSettings = (props: { url: string }) => {
	// 組合需要的 api
	const api = BackendUrl(props.url, ApiDef.GET_DEVICE_STATE);
}
```

當然，BackendUrl 就要多一個參數，但是可以解決 build 的問題：
```js
const BackendUrl = (url: string, api: number, query: QueryObject = {}) => {
...
}

```

雖然比較搞鋼，目前沒有找到可以解決 build 的方法，只好將就一下。
 