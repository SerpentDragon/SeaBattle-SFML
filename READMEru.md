# Морской бой
Данное приложение является классической игрой "Морской бой", которая была создана с помощью кроссплатформенной графической библиотеки SFML 2.5.1

# Правила игры
Игровое поле имеет размер 10x10 клеток. Каждый игрок размещает свои корабли на своей стороне игровой доски. Корабли могут быть разной длины, но не должны перекрываться или соприкасаться друг с другом.

Когда игроки закончили расставлять корабли, игроки начинают поочередно атаковать корабли противника. Каждый игрок выбирает клетку на доске, на которую он хочет произвести выстрел, и сообщает адрес этой клетки противнику.

Если выстрел попадает в одну из клеток корабля противника, противник сообщает об этом, и атакующий игрок отмечает клетку на своей стороне доски как "попадание". Если корабль противника уничтожен, то игрок отмечает все клетки корабля как "потопленные".

Если выстрел промахивается, атакующий игрок отмечает клетку на своей стороне доски как "промах". Игроки продолжают атаковать по очереди до тех пор, пока один из игроков не уничтожит все корабли противника.

Игрок, который первым уничтожит все корабли противника, побеждает игру.

## Функционал
Пользователь играет против компьютера.
Перед началом игры пользователь размещает свои корабли на поле путём перетаскивания. Для того, чтобы изменить ориентацию корабля, необходимо нажать на нём ПКМ, при этом удерживая его ЛКМ. Кроме того, пользователю предоставляется возможность автоматической расстановки своих кораблей. Для этого необходимо нажать соответствующую кнопку. Когда все корабли расставлены, нужно нажать кнопку "Старт" - игра начнётся

## Сборка и запуск
Для сборки и запуска из локальной директории можно использовать приведенный CMakeLists.txt:
```
git clone git@github.com:SerpentDragon/SeaBattle-SFML.git
cd SeaBattle-SFML
mkdir build
cd build
cmake ..
cmake --build .
./sea_battle
```
Для установки приложения на Ваш ПК, Вы можете скачать релиз из ветки'release_branch' и установить его:
```
sudo dpkg -i SEA_BATTLE-1.0.0-Linux.deb
```
Для корректной работы приложения выполнить
```
sudo sea_battle
```

## Удаление
Чтобы удалить установленное приложение, выполните:
```
sudo dpkg -r sea_battle
sudo rm -rf /usr/local/SEA_BATTLE
```

## Демо
![Alt text](https://github.com/SerpentDragon/SeaBattle-SFML/blob/master/Demo/demo.png)
