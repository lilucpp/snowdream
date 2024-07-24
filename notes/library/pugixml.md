# pugixml
1. 增加cdata
https://github.com/zeux/pugixml/issues/169
```c++
node.append_child(pugi::node_cdata).set_value("Some text in CDATA format");
```
获取cdata，直接node.text();
修改cdata，node.text().set();