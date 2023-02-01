const express = require('express');
const bodyParser = require("body-parser");
const app = express();
const port = 3000;
const path = require('path');
const addon = require('bindings')('emit_from_cpp')
const htmlPage = `<!DOCTYPE html>
<html>
<head>
    <title>Результат</title>
</head>
<body>
    <h3>text</h3>
    <a href="/">Назад</a>
</body>
<html>`

app.listen(port, () => {
  console.log(`http://localhost:${port}`)
})

app.use(bodyParser.urlencoded({ extended: false }));
app.use(bodyParser.json());

app.get('/', (req, res) => {
  res.sendFile(path.join(__dirname,'/html/form.html'))
})

app.post('/form', (req, res) => {
  if (addon.checkUser(req.body.username)) {
    res.send(htmlPage.replace("text", 'Пользователь '+req.body.username+' есть'));
  } else {
    res.send(htmlPage.replace("text", 'Пользователя '+req.body.username+' нет'));
  } 
})





