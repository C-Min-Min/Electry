# Electry
<img src="/assets/images/electry_logo.png" width="360">

# Описание:
Electry е смарт електромер, който ще Ви помогне да следите и да пестите Вашата консумация на електричество.  
С красиво изградените Android и Web приложения, Вие ще можете да комуникирате с Вашия електромер, който има функцията да разпознава употребяваните от Вас консуматори чрез невронна мрежа.
## Презентация:
### Google Drive:
Презентация + Видео
Линк към Google Drive -> [Натиснете тук, за да отидете към папката](https://drive.google.com/drive/folders/1UAdsdcYnGQrwe6Rr6VAjdduheQH5-I9Q?usp=sharing)
### PowerPoint:
Линк към PowerPoint Online -> [Натиснете тук, за да видите презентацията](https://drive.google.com/drive/folders/1UAdsdcYnGQrwe6Rr6VAjdduheQH5-I9Q?usp=sharing)

## Как да си изтегля и използвам проекта?

### Инструкции за изтегляне и инсталиране на web приложението:
#### От сорс код:
```
git clone https://github.com/C-Min-Min/Electry/
cd Electry/web_app
npm install
npm start
```
```
node server.js
```
```
cd node-mysql
node connection.js
```
#### От пре-компилираните файлове:
* Изтеглете папката 'web_app_build' от последната версия
```
cd code ./web_app_build
node server.js
```
```
cd node-mysql
node connection.js
```
* Пуснете live сървъра си на MS/VS-Code
### Инструкции за изтегляне и инсталиране на Android приложението:
#### От сорс код:
* Стъпка 1: Отворете Android Studio
* Стъпка 2: Натиснете бутона 'Build'
* И използвайте APK-то
#### От пре-компилирания файл (с използване на Android телефон):
* Стъпка 1: Изтеглете (от последната версия) файлът 'electry.apk'
* Стъпка 2: Отидете във Вашата 'Downloads' папка:
  <br /><img src="/assets/images/android_app/step2.png" width="360"> 
* Стъпка 3: Отворете (натиснете) apk файлът:
  <br /><img src="/assets/images/android_app/step3.png" width="360">
* За 3 и 4 стъпка може да не са Ви нужни, ако са направени преди
* Стъпйа 4: Ще Ви излезе прозорец, който ще Ви каже, че на източникът се няма доверие да инсталира приложения, отворете настройките: 
  <br /><img src="/assets/images/android_app/step4.png" width="360">
* Стъпка 5: Включете опцията 'Позволете от този източник:'
  <br /><img src="/assets/images/android_app/step5.png" width="360">
* Стъпка 6: След всичко това, ще Ви излезе прозорец, в който ще може да инсталирате приложението: 
  <br /><img src="/assets/images/android_app/step6.png" width="360">
* Стъпка 7: След като инсталирането свърши (успешно) ще можете или да го оставите, или да го отворите (препоръчително):
  <br /><img src="/assets/images/android_app/step7.png" width="360">
* Стъпка 8: Наслаждавайте се! 
## Използвани технологии:

#### Web App
* [HTML](https://html.com/)
* [CSS](https://www.w3.org/Style/CSS/Overview.en.html)
* [JavaScript](https://www.javascript.com/)
* [ReactJS](https://reactjs.org/)
* [NodeJS](https://nodejs.org/)
#### Android App
* [JAVA](https://www.java.com/)
* [PHP](https://www.php.net/)
* [MySQL](https://www.mysql.com/)
#### Hardware
* [MCP39F511](http://ww1.microchip.com/downloads/en/DeviceDoc/20005393B.pdf)
* [STM32F407](https://www.st.com/en/microcontrollers-microprocessors/stm32f407-417.html)
* [ESP32](https://www.espressif.com/en/products/socs/esp32)
* [C-The-Lang](https://port70.net/~nsz/c/c11/n1570.html)

## Информация за авторите на проекта:

* [Калоян Дойчинов](https://kaloyan.tech) | WebApp
* [Мартин Божилов](https://github.com/TechXTT) | Mobile App
* [Васил Колев](https://github.com/AvatarBg111) | Hardware
