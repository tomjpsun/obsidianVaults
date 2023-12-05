[Refrence Here](https://stackoverflow.com/questions/46063693/how-can-i-get-data-from-a-local-file-into-my-react-app)

用   [Raw-loader](https://github.com/webpack-contrib/raw-loader),then
```js
import txt from 'file.txt';
```

還沒試驗，有此一說：
```javascript
<input type=file></input> 
```

用 [Raw.Macro][https://github.com/pveyes/raw.macro]， then
```javascript
  import raw from 'raw.macro';

    function foo(){
        const jsonContent = raw('../utils/stops.json');
        console.log(jsonContent);
    }
```

