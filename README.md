# TransportСatalogue
## Транспортный справочник
Программная реализация системы маршрутизации, работающая с JSON запросами. Программа двухстадийная и имеет два варианта запросов, первый отвечает за создание базы данных, второй же выполняет всю нужную работу для того, чтобы выдать нужную информацию по любым оптимальным маршрутам, автобусам или остановкам. Формата вывода: .json и .svg.  

По запросу "make_base" из входного потока считывает маршруты автобусов, остановки и настройки отрисовки, обрабатывает эти данные и с помощью серилизации сохраняет состояние.  
По запросу "process_requests" из входного потока считывает требуемый запрос, и согласно нему в выходной поток выводит информацию:  
__для автобуса__
- длину маршрута автобуса
- колличество остановок
- колличество уникальных остановок
- извилистость маршрута. Она равна отношению длины дорожного расстояния маршрута к длине географического расстояния.  
 
__для остановки__
- автобусы проходящие через эту остановку  

__для маршрута__   
_вывод информации повторяется для каждой остановки на которой нужно сесть/пересесть в автобус и для каждого требуемого автобуса
маршрут выбирается исходя из мимнимального времени в пути_ 
- название остановки начала маршрута
- время ожидания автобуса
- номер необходимого автобуса 
- колличество остановок необходимое проехать на этом автобусе до выхода 
- затрачивоемое время в движении на этом автобусе
- остановки для пересадок (если требуется) 

- общее время в пути  

__для карты__  
- строки с svg изображением

## Использование:
Программа должна запускаться с аргументом командной строки "make_base" или "process_requests".  
После запуска с аргументом командной строки "make_base" в стандортный поток ввода необходимо подать документ содержащий данные для создания справочника (пример запроса ниже).  
Чтобы ввести запрос программу необходимо запустить с аргументом командной строки "process_requests" и в стандортный поток ввода необходимо подать документ содержащий требуемые запросы после чего программа выведет данные согласно запросу (пример запроса ниже). 

<details><summary>Пример ввода после команды "make_base"</summary>

~~~
{
      "serialization_settings": {
          "file": "transport_catalogue.db"
      },
      "routing_settings": {
          "bus_wait_time": 2,
          "bus_velocity": 30
      },
      "render_settings": {
          "width": 1200,
          "height": 500,
          "padding": 50,
          "stop_radius": 5,
          "line_width": 14,
          "bus_label_font_size": 20,
          "bus_label_offset": [
              7,
              15
          ],
          "stop_label_font_size": 18,
          "stop_label_offset": [
              7,
              -3
          ],
          "underlayer_color": [
              255,
              255,
              255,
              0.85
          ],
          "underlayer_width": 3,
          "color_palette": [
              "green",
              [
                  255,
                  160,
                  0
              ],
              "red"
          ]
      },
      "base_requests": [
          {
              "type": "Bus",
              "name": "14",
              "stops": [
                  "street Liza Chaikina",
                  "Power grid",
                  "R bridge",
                  "Hotel Sochi",
                  "K street",
                  "Need",
                  "street D",
                  "street Liza Chaikina"
              ],
              "is_roundtrip": true
          },
          {
              "type": "Bus",
              "name": "24",
              "stops": [
                  "street D",
                  "P street",
                  "Power grid",
                  "Russia"
              ],
              "is_roundtrip": false
          },
          {
              "type": "Bus",
              "name": "114",
              "stops": [
                  "Marine station",
                  "R bridge"
              ],
              "is_roundtrip": false
          },
          {
              "type": "Stop",
              "name": "street Liza Chaikina",
              "latitude": 43.590317,
              "longitude": 39.746833,
              "road_distances": {
                  "Power grid": 4300,
                  "street D": 2000
              }
          },
          {
              "type": "Stop",
              "name": "Marine station",
              "latitude": 43.581969,
              "longitude": 39.719848,
              "road_distances": {
                  "R bridge": 850
              }
          },
          {
              "type": "Stop",
              "name": "Power grid",
              "latitude": 43.598701,
              "longitude": 39.730623,
              "road_distances": {
                  "Russia": 4500,
                  "P street": 1200,
                  "R bridge": 1900
              }
          },
          {
              "type": "Stop",
              "name": "R bridge",
              "latitude": 43.587795,
              "longitude": 39.716901,
              "road_distances": {
                  "Marine station": 850,
                  "Hotel Sochi": 1740
              }
          },
          {
              "type": "Stop",
              "name": "Hotel Sochi",
              "latitude": 43.578079,
              "longitude": 39.728068,
              "road_distances": {
                  "K street": 320
              }
          },
          {
              "type": "Stop",
              "name": "K street",
              "latitude": 43.578509,
              "longitude": 39.730959,
              "road_distances": {
                  "Need": 370
              }
          },
          {
              "type": "Stop",
              "name": "Need",
              "latitude": 43.579285,
              "longitude": 39.733742,
              "road_distances": {
                  "street D": 600
              }
          },
          {
              "type": "Stop",
              "name": "street D",
              "latitude": 43.585586,
              "longitude": 39.733879,
              "road_distances": {
                  "P street": 1100
              }
          },
          {
              "type": "Stop",
              "name": "P street",
              "latitude": 43.590041,
              "longitude": 39.732886,
              "road_distances": {}
          },
          {
              "type": "Stop",
              "name": "Russia",
              "latitude": 43.601202,
              "longitude": 39.715498,
              "road_distances": {}
          }
      ]
  }
~~~
</details>

<details><summary>Пример ввода после команды "process_requests"</summary>
      
~~~
{
      "serialization_settings": {
          "file": "transport_catalogue.db"
      },
      "stat_requests": [
          {
              "id": 218563507,
              "type": "Bus",
              "name": "14"
          },
          {
              "id": 508658276,
              "type": "Stop",
              "name": "Power grid"
          },
          {
              "id": 1964680131,
              "type": "Route",
              "from": "Marine station",
              "to": "P street"
          },
          {
              "id": 1359372752,
              "type": "Map"
          }
      ]
  }
~~~
</details>

<details><summary>Вывод после команды "process_requests"</summary>

~~~
[
    {
        "curvature": 1.60481,
        "request_id": 218563507,
        "route_length": 11230,
        "stop_count": 8,
        "unique_stop_count": 7
    },
    {
        "buses": [
            "14",
            "24"
        ],
        "request_id": 508658276
    },
    {
        "items": [
            {
                "stop_name": "Marine station",
                "time": 2,
                "type": "Wait"
            },
            {
                "bus": "114",
                "span_count": 1,
                "time": 1.7,
                "type": "Bus"
            },
            {
                "stop_name": "R bridge",
                "time": 2,
                "type": "Wait"
            },
            {
                "bus": "14",
                "span_count": 4,
                "time": 6.06,
                "type": "Bus"
            },
            {
                "stop_name": "street D",
                "time": 2,
                "type": "Wait"
            },
            {
                "bus": "24",
                "span_count": 1,
                "time": 2.2,
                "type": "Bus"
            }
        ],
        "request_id": 1964680131,
        "total_time": 15.96
    },
    {
        "map": "<?xml version="1.0" encoding="UTF-8" ?>
<svg xmlns="http://www.w3.org/2000/svg" version="1.1">
  <polyline points="125.25,382.708 74.2702,281.925 125.25,382.708" fill="none" stroke="green" stroke-width="14" stroke-linecap="round" stroke-linejoin="round"/>
  <polyline points="592.058,238.297 311.644,93.2643 74.2702,281.925 267.446,450 317.457,442.562 365.599,429.138 367.969,320.138 592.058,238.297" fill="none" stroke="rgb(255,160,0)" stroke-width="14" stroke-linecap="round" stroke-linejoin="round"/>
  <polyline points="367.969,320.138 350.791,243.072 311.644,93.2643 50,50 311.644,93.2643 350.791,243.072 367.969,320.138" fill="none" stroke="red" stroke-width="14" stroke-linecap="round" stroke-linejoin="round"/>
  <text x="125.25" y="382.708" dx="7" dy="15" font-size="20" font-family="Verdana" font-weight="bold" fill="rgba(255,255,255,0.85)" stroke="rgba(255,255,255,0.85)" stroke-width="3" stroke-linecap="round" stroke-linejoin="round">114</text>
  <text x="125.25" y="382.708" dx="7" dy="15" font-size="20" font-family="Verdana" font-weight="bold" fill="green">114</text>
  <text x="74.2702" y="281.925" dx="7" dy="15" font-size="20" font-family="Verdana" font-weight="bold" fill="rgba(255,255,255,0.85)" stroke="rgba(255,255,255,0.85)" stroke-width="3" stroke-linecap="round" stroke-linejoin="round">114</text>
  <text x="74.2702" y="281.925" dx="7" dy="15" font-size="20" font-family="Verdana" font-weight="bold" fill="green">114</text>
  <text x="592.058" y="238.297" dx="7" dy="15" font-size="20" font-family="Verdana" font-weight="bold" fill="rgba(255,255,255,0.85)" stroke="rgba(255,255,255,0.85)" stroke-width="3" stroke-linecap="round" stroke-linejoin="round">14</text>
  <text x="592.058" y="238.297" dx="7" dy="15" font-size="20" font-family="Verdana" font-weight="bold" fill="rgb(255,160,0)">14</text>
  <text x="367.969" y="320.138" dx="7" dy="15" font-size="20" font-family="Verdana" font-weight="bold" fill="rgba(255,255,255,0.85)" stroke="rgba(255,255,255,0.85)" stroke-width="3" stroke-linecap="round" stroke-linejoin="round">24</text>
  <text x="367.969" y="320.138" dx="7" dy="15" font-size="20" font-family="Verdana" font-weight="bold" fill="red">24</text>
  <text x="50" y="50" dx="7" dy="15" font-size="20" font-family="Verdana" font-weight="bold" fill="rgba(255,255,255,0.85)" stroke="rgba(255,255,255,0.85)" stroke-width="3" stroke-linecap="round" stroke-linejoin="round">24</text>
  <text x="50" y="50" dx="7" dy="15" font-size="20" font-family="Verdana" font-weight="bold" fill="red">24</text>
  <circle cx="267.446" cy="450" r="5" fill="white"/>
  <circle cx="317.457" cy="442.562" r="5" fill="white"/>
  <circle cx="125.25" cy="382.708" r="5" fill="white"/>
  <circle cx="365.599" cy="429.138" r="5" fill="white"/>
  <circle cx="350.791" cy="243.072" r="5" fill="white"/>
  <circle cx="311.644" cy="93.2643" r="5" fill="white"/>
  <circle cx="74.2702" cy="281.925" r="5" fill="white"/>
  <circle cx="50" cy="50" r="5" fill="white"/>
  <circle cx="367.969" cy="320.138" r="5" fill="white"/>
  <circle cx="592.058" cy="238.297" r="5" fill="white"/>
  <text x="267.446" y="450" dx="7" dy="-3" font-size="18" font-family="Verdana" fill="rgba(255,255,255,0.85)" stroke="rgba(255,255,255,0.85)" stroke-width="3" stroke-linecap="round" stroke-linejoin="round">Hotel Sochi</text>
  <text x="267.446" y="450" dx="7" dy="-3" font-size="18" font-family="Verdana" fill="black">Hotel Sochi</text>
  <text x="317.457" y="442.562" dx="7" dy="-3" font-size="18" font-family="Verdana" fill="rgba(255,255,255,0.85)" stroke="rgba(255,255,255,0.85)" stroke-width="3" stroke-linecap="round" stroke-linejoin="round">K street</text>
  <text x="317.457" y="442.562" dx="7" dy="-3" font-size="18" font-family="Verdana" fill="black">K street</text>
  <text x="125.25" y="382.708" dx="7" dy="-3" font-size="18" font-family="Verdana" fill="rgba(255,255,255,0.85)" stroke="rgba(255,255,255,0.85)" stroke-width="3" stroke-linecap="round" stroke-linejoin="round">Marine station</text>
  <text x="125.25" y="382.708" dx="7" dy="-3" font-size="18" font-family="Verdana" fill="black">Marine station</text>
  <text x="365.599" y="429.138" dx="7" dy="-3" font-size="18" font-family="Verdana" fill="rgba(255,255,255,0.85)" stroke="rgba(255,255,255,0.85)" stroke-width="3" stroke-linecap="round" stroke-linejoin="round">Need</text>
  <text x="365.599" y="429.138" dx="7" dy="-3" font-size="18" font-family="Verdana" fill="black">Need</text>
  <text x="350.791" y="243.072" dx="7" dy="-3" font-size="18" font-family="Verdana" fill="rgba(255,255,255,0.85)" stroke="rgba(255,255,255,0.85)" stroke-width="3" stroke-linecap="round" stroke-linejoin="round">P street</text>
  <text x="350.791" y="243.072" dx="7" dy="-3" font-size="18" font-family="Verdana" fill="black">P street</text>
  <text x="311.644" y="93.2643" dx="7" dy="-3" font-size="18" font-family="Verdana" fill="rgba(255,255,255,0.85)" stroke="rgba(255,255,255,0.85)" stroke-width="3" stroke-linecap="round" stroke-linejoin="round">Power grid</text>
  <text x="311.644" y="93.2643" dx="7" dy="-3" font-size="18" font-family="Verdana" fill="black">Power grid</text>
  <text x="74.2702" y="281.925" dx="7" dy="-3" font-size="18" font-family="Verdana" fill="rgba(255,255,255,0.85)" stroke="rgba(255,255,255,0.85)" stroke-width="3" stroke-linecap="round" stroke-linejoin="round">R bridge</text>
  <text x="74.2702" y="281.925" dx="7" dy="-3" font-size="18" font-family="Verdana" fill="black">R bridge</text>
  <text x="50" y="50" dx="7" dy="-3" font-size="18" font-family="Verdana" fill="rgba(255,255,255,0.85)" stroke="rgba(255,255,255,0.85)" stroke-width="3" stroke-linecap="round" stroke-linejoin="round">Russia</text>
  <text x="50" y="50" dx="7" dy="-3" font-size="18" font-family="Verdana" fill="black">Russia</text>
  <text x="367.969" y="320.138" dx="7" dy="-3" font-size="18" font-family="Verdana" fill="rgba(255,255,255,0.85)" stroke="rgba(255,255,255,0.85)" stroke-width="3" stroke-linecap="round" stroke-linejoin="round">street D</text>
  <text x="367.969" y="320.138" dx="7" dy="-3" font-size="18" font-family="Verdana" fill="black">street D</text>
  <text x="592.058" y="238.297" dx="7" dy="-3" font-size="18" font-family="Verdana" fill="rgba(255,255,255,0.85)" stroke="rgba(255,255,255,0.85)" stroke-width="3" stroke-linecap="round" stroke-linejoin="round">street Liza Chaikina</text>
  <text x="592.058" y="238.297" dx="7" dy="-3" font-size="18" font-family="Verdana" fill="black">street Liza Chaikina</text>
</svg>",
        "request_id": 1359372752
    }
~~~
</details>

## Cистемные требования:
- С++17 (STL);
- GCC (MinGW-w64) 11.2.0.

## Стек технологий:
- CMake 3.22.0;
- Protobuf-cpp 3.18.1.

