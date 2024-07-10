# Визуализация картинки 3D с передачей с сеть

**Сборка**

`git clone https://github.com/m-khomutov/ocean3d.git`  
`cd ocean3d/`  
`mkdir build`  
  
Возможные варианты сборки:  
* с использованием toolkit-a Qt  
`.qmake`  
`make all`  
  
* с использованием cmake  
`cd build/`  
`cmake ..`  
`make all`  

**Установка с использованием toolkit-a Qt**  
  
* Файл запуска эмулятора  
  >build/simulator/app/simulator  
* Файл запуска программы просмотра  
  >build/viewer/app/viewer  
  
**Установка с использованием cmake**  
* Файл запуска эмулятора  
  >build/simulator/simulator  
* Файл запуска программы просмотра  
  >build/viewer/viewer  

**Запуск (пример)**
  
* Эмулятор с парамертами  
 `simulator -s ../../../simulator/shaders/ -d 40 -w 800x600 -t ../../../simulator/textures`  
  
* Эмулятор с файлом конфигурации  
 `simulator -c /opt/agat-a/etc/conf`  
  
* Программа просмотрa  
 `viewer -u http://127.0.0.1:2232/0`  

**Параметры запуска (выводятся опцией -h)**  

* Эмулятор  
  
```
$ simulator -h  
Вывод опций программы  
Запуск: ./simulator[-h] [-s] [-t] [-p] [-q] [-d] [-w] [-o] [-c] 
  
Эмулятор устройства  
   
обязательные аргументы:  
 	  -s	каталог с шейдерами  
 	  -t	каталог с текстурами  
Опциональные аргументы:  
 	  -p	порт прослушки (def. 2232)  
 	  -w	размеры окна (def. 800x600)  
 	  -q	качество сжатия % (def. 80)  
 	  -d	длительность кадра мс (def. 40)  
 	  -c	файл конфигурации  
 	  -o	каталог с объектами blender  
 	  -h	вывод параметров запуска  
  ```
* Программа просмотрa  
  
```
$ viewer -h  
Вывод опций программы  
Запуск: ./viewer[-h] [-u] [-w] [-v]  
   
программа просмотра  
  
обязательные аргументы:  
          -u	url эмулятора (протокол//:хост:порт/номер_стрима) 
Опциональные аргументы:  
 	  -w	размеры окна (def. 800x600)  
 	  -v	вывод оценки задержки (def. false)  
 	  -h	вывод параметров запуска  
  ```

**Файл конфигурации**  
  
Каждый параметр занимает строку файла.  
Строки, начинающиеся с символа # игнорируются (являются комментариями).  
Формат строки: ключ=значение, без пробелов, строковые значения в двойных кавычках.  
  
Пример файла:  
```
# Файл конфигурации эмулятора вида на море

shaders="/opt/agat-a/shaders"               # каталог шейдеров
textures="/opt/agat-a/textures"             # каталог текстур 
objs="/opt/agat-a/objs"                     # каталог объектов, выгруженных из blender-a в формате OBJ
scenes="/opt/agat-a/scenes"                 # каталог сцен
screen_layout="/opt/agat-a/etc/layout.json" # расположение воспроизводимых сцен
port=2232                                   # порт приема запросов на трансляцию стрима
window=800x600                              # размеры окна
quality=80                                  # качество сжатия %
duration=40                                 # длительность кадра мсек.
```
  
**Каталог сцен**  
  
Содержит набор файлов, каждый из которых является спецификацией сцены.  

***Спецификация сцены***  
  
Файл из каталога сцен. Состоит из блоков. Блоки могут повторяться. Каждый блок начинается заголовком в квадратных скобках.  
За заголовком следует набор параметров в формате ключ=значение, без пробелов, строковые значения в двойных кавычках, векторы в фигурных скобках, через пробел.  
Каждый параметр занимает строку файла.  
Строки, начинающиеся с символа # игнорируются (являются комментариями).  
  
Поддерживаются следующие блоки:
* [Environment]  
  `общие настройки остальных объектов`

* [Horizon]  
  `подложка сцены (не используется совместно с Water и Sol)`

* [Water]  
  `водная поверхность (не используется совместно с Horizon и Sparklets. Используется совместно с Sol)`

* [Sparklets]  
  `водная поверхность с бликами (не используется совместно с Horizon и Water. Используется совместно с Sol)`

* [Sol]  
  `небо над водной поверхностью (не используется совместно с Horizon. Используется совместно с Water)`

* [Vessel]  
  `фигура корабля`

Пример спецификации: водная поверхность, небо и одна фигура корабля (подложка закомментирована):  
```
[Environment]
# координаты наблюдения (местоположение камер в мировых координатах)
camera_position={{0.4 0.0 3.0} {-0.4 0.0 3.0}}
# цвет источника освещения 
light_color={1.0 1.0 0.75}
# местоположение в сцене источника освещения 
light_position={0.0 0.5 -3.0}
# цвет тумана
fog_color={0.892, 0.892, 0.872 1.0}
# плотность тумана
fog_density=0.04
   
#[Horizon]
# имя файла шейдера. Сам файл должен находиться в каталоге, определенном в файле конфигурации.
#shader="horizon.glsl"
# имя файла текстуры. Сам файл должен находиться в каталоге, определенном в файле конфигурации.
#texture="56118_Peaceful_ocean_background_HD_BG.avi"
# экран отображения фона. Точки: левая верхняя, правая верхняя, правая нижняя, левая нижняя   
#viewport={{-1.5 1.0 -1.0} {1.5 1.0 -1.0} {1.5 -1.0 0.66} {-1.5 -1.0 0.66}} # {topleft topright bottomright bottomleft}   
   
#[Sparklets]
# имя файла шейдера. Сам файл должен находиться в каталоге, определенном в файле конфигурации.
#shader="sparklets.glsl"
# имя файла текстуры. Сам файл должен находиться в каталоге, определенном в файле конфигурации.
#texture="reflection.jpg"
# имя файла текстуры прозрачности воды. Сам файл должен находиться в каталоге, определенном в файле конфигурации.
#alpha="alpha.jpg"
# имя файла текстуры отражаемого неба. Сам файл должен находиться в каталоге, определенном в файле конфигурации.
#air="air.jpg"
# параметры генерации волн: амплитуда, частота в горизонтальной и вертикальной плоскостях.
#surge={{0.01 0.2} {0.01 0.4}} # horizontal (amplitude, frequency) vertical (amplitude, frequency)
# экран отображения. Точки: левая верхняя, правая верхняя, правая нижняя, левая нижняя   
#viewport={{-1.3 0.6 -0.1} {1.3 0.6 -0.1} {1.0 -1.8 0.1} {-1.0 -1.8 0.1}} # {topleft topright bottomright bottomleft}
# скорость распространения волны в горизонтальном направлении (остальные направления не используются)
#speed={0.003 0.0 0.0}
# шаг отрисовки блесток
#step=0.03
  
[Water]
# имя файла шейдера. Сам файл должен находиться в каталоге, определенном в файле конфигурации.
shader="water.glsl"
# имя файла текстуры. Сам файл должен находиться в каталоге, определенном в файле конфигурации.
texture="reflection.jpg"
# имя файла текстуры прозрачности воды (опционно). Сам файл должен находиться в каталоге, определенном в файле конфигурации.
alpha="alpha.jpg"
# имя файла текстуры отражаемого неба. Сам файл должен находиться в каталоге, определенном в файле конфигурации.
air="air.jpg"
# имя файла текстуры пенного следа. Сам файл должен находиться в каталоге, определенном в файле конфигурации.
foam="foam.jpg"
# параметры генерации волн: амплитуда, скорость
wave={10.0 2.6}
# параметры генерации волн следа корабля: амплитуда, скорость
wake={20.0 5.0}
  
[Sol]
# имя файла шейдера. Сам файл должен находиться в каталоге, определенном в файле конфигурации.
shader="sol.glsl"
# имя файла текстуры. Сам файл должен находиться в каталоге, определенном в файле конфигурации.
texture="air.jpg"
# экран отображения неба. Точки: левая верхняя, правая верхняя, правая нижняя, левая нижняя   
viewport={{-5.0 3.5 -10.0} {9.9 3.5 -10.0} {9.9 -1.6 -10.0} {-5.0 -1.6 -10.0}} # {topleft topright bottomright bottomleft}
  
[Vessel]
# имя файла шейдера. Сам файл должен находиться в каталоге, определенном в файле конфигурации.
shader="vessel.glsl"
# имя файла объекта, выгруженного из blender-a. Сам файл должен находиться в каталоге, определенном в файле конфигурации.
object="vessel.obj"
# общая скорость движения по сцене
speed={0.00035 -0.00015 0.0}
# точка начала движения по сцене
start_position={-1.2 0.21 0.0}
# углы, определяющие курс
course={-90.0 0.0 0.0}
# длина перемещения по координате х сцены
trajectory=2.62
# углы максимального отклонения качки
pitching_range={15.0 22.0}
# изменение угла качки в моменте
pitching=0.1
# изменение текущих углов в процессе движения
angle_gain={0.0 0.0 0.0}
# фактор размера в начале движения
start_factor={0.05 0.05 0.05}
# скорость изменения фактора в процессе движения
factor_gain=0.00005
# параметры следа на воде: начальная ширина, скорость расширения
wake_width={0.3 0.3}
```   
   
**Каталоги проекта**  
  
Местоположение каталогов прописывается в файле конфигурации. Используются следующие каталоги:  
* objs  
  Каталог OBJ-файлов и MTL-файлов, получаемых из программы 3D-конструирования blender через экспорт объектов.  
* scenes  
  Каталог SCN-файлов, являющихся спецификацией сцены.  
* shaders  
  Каталог шейдеров, используемых для отрисовки объектов.  
* textures  
  Каталог текстур. Поддерживаются текстуры статических изображений (PNG, JPEG) и текстуры видео (AVI/MJPEG).  
  
**REST api эмулятора**  
  
Поддерживаются следующие команды REST api:  
* scene?list  
  Вывести список имеющихся у эмулятора сцен. Выводится список имен файлов каталога **scenes** без расширения **.scn**  
  `wget -O list 'http://127.0.0.1:2232/scene?list'`  
  Ответ:  
```
{
  "success": true,
  "scenes": [
    "tug",
    "wakeboard",
    "cruiser"
  ]
}
```  
* scene?get  
  Вывести текущую сцену  
  `wget -O scene 'http://127.0.0.1:2232/scene?get'`  
  Ответ:  
```
{
  "success": true,
  "scene": "wakeboard"
}
```  
* scene?set=имя_сцены  
  Сменить текущую сцену на предлагаемую  
  `wget -O scene 'http://127.0.0.1:2232/scene?set=tug'`  
  Ответ:  
```
{
  "success": true,
  "scene": "tug"
}
```  
  
  
**Протокол передачи медиа данных**

Используется не совсем стандартный протокол **Flash Video**. Имеются следующие несоответствия стандарту:
* Поддержка кодека JPEG. Стандарт определяет как  
 ` JPEG (currently unused)`
* timestamp передается как 8-ми байтная переменная **uint64_t**. Содержит текущую UTC-метку
 Определение стандарта
```
Timestamp UI24 
TimestampExtended UI8
```
* поле Stream ID удалено.

Заголовок, таким образом имеет структуру

| Поле  | Размер |
| --- | --- |
| TagType  | UI8  |
| DataSize  | UI24  |
| Timestamp  | UI64  |
| Data  | VIDEODATA  |

**Библиотека протокола передачи медиа данных**

В проекте собирается и используется статическая библиотека **libkformat.a**.
В библиотеке имеется реализация используемого протокола передачи медиа данных.

***Использование библиотечного функционала***

Классы библиотеки
* baseprotocol - интерфейс взаимодействия с классом передачи медиа данных
* flvprotocol - реализация интерфейса baseprotocol для протокола **Flash Video**.

****baseprotocol****

Содержит следующие функции-члены
* virtual void on_data( const uint8_t * data, int size ) = 0; Вызывается при наличии на сокете данных для получения
* virtual void do_write() = 0; - Вызывается для допередачи данных при освобождении места в tcp-буферах сокета
* virtual void send_frame( const uint8_t * data, int size, float duration ) = 0; - Вызывается для отправки фрейма данных
* virtual bool can_send_frame() const = 0; - Используется ли протокол для отправки медиа контента (протокол может быть служебным для передачи команд)

****flvprotocol****

Реализует интерфейс baseprotocol для протокола передачи **Flash Video**

***Пример использования***

В качестве примера используется применение flvprotocol с функционалом epoll
```
int fd_count = epoll_wait( fd_, events, maxevents, 40 );
for( int i(0); i < fd_count; ++i )
{
    if( events[i].data.fd == socket_ )
    {
        . . .
        std::unique_ptr< flvprotocol > proto;
        . . .
        int fd = accept( events[i].data.fd, . . . );
        proto.reset( new flvprotocol( fd ) );
        . . .
    }
    else if( events[i].events & EPOLLIN )
    {
         int rc = ::read( events[i].data.fd, buffer, sizeof(buffer) );
         if( rc > 0 )
         {
             . . .
             {
                 proto->on_data( buffer, rc );
             }
         }
     }
     else if( events[i].events & EPOLLOUT )
     {
         . . .
         {
             proto->on_ready_to_write();
         }
     } 
}
. . .
if( std::chrono::duration_cast< std::chrono::milliseconds >(d) >= frame_duration_ )
{
    . . .
    proto->send_frame( frame.data(), frame.size() )
    . . .
}

```

В каталоге **examples** находятся файлы
* example_frame.cpp
* example_proto.cpp
* example_service.cpp

Это простейшие примеры использования функционала библиотеки.

Примеры собираются командой
```
g++ -o example example_[frame|proto|service].cpp -I.. ../../build/kformat/lib/libkformat.a
```


Сборка должна производиться в каталоге **examples**.

***example_proto***

Самый простой пример использования абстракции представления протокола **flvprotocol**. 
Абстракция реализует протокол передачи **Flash Video**.
Запускается с одним аргументом - путь к файлу jpeg.
Открывает сокет tcp и слушает входные запросы на порт 5555.
Oтправляет программе просмотра содержимое файла jpeg с периодом 40 мс.

***example_frame***

Самый простой пример использования абстракции представления протокола **flvprotocol** и 
абстракции представления буфера фрейма **jpegframe**. 
Абстракция реализует сжатие видеофрейма с последующей передачей по протоколу **Flash Video**.
Отправляет программе просмотра сгенерированное изображение красной полосы, двигающейся по экрану.

***example_service**

Самый простой пример использования абстракции представления протокола **flvprotocol**, 
абстракции представления буфера фрейма **jpegframe** и абстракции сервиса предоставления видеопотока
**service** (класс, производный от класса **baseservice**).
После запуска сервиса приложение периодически обновляет фрейм на передачу.
Отправляет программе просмотра сгенерированное изображение красной полосы, двигающейся по экрану.

