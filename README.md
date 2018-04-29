# NomaCh (Noma_blockChain)

Karama Hakaton project #0111

Автор: Максим Скворцов

e-mail: depizzottri@gmail.com

tg: depizzottri

Проект #0111 для хакатона Karma.red

Для того чтобы полностью понять весть этот проект следует прочитать

[Полное описание](docs/README.md)

следуя по cсылкам по ходу чтения.

Отдельно можно приступить к описанию кода

[Описание кода](docs/P2P.md)

Инструкция по сборке:

* Клонировать этот репозитарий и перейти в него
* ```git submodule update --init --recursive libs/restbed```
* ```git submodule update --init libs/actor-framework```
* ```git submodule update --init libs/delta-enabled-crdts```

Необходимо иметь установленный Boost (тестировалось с 1.58 и 1.62).

Сборка под Windows (пример):

```mkdir build && cd build```
```cmake -G "Visual Studio 14 2015 Win64" -DBOOST_ROOT:PATH=C:/lib/boost_1_62_0 -DBOOST_INCLUDEDIR:PATH=C:/local/boost_1_62_0/boost -DBOOST_LIBRARYDIR:PATH=C:/lib/boost_1_62_0/stage_x64/lib ..```
Проекты для сборки и запуска: noma_tests, noma_http, noma_node_random_add, noma_peer_sampling, nomadb

(почемуто не работает с 1.66)

Сборка под Linux:

```mkdir build && cd build && cmake .. && make```

Тестировано на следующих системах:

Windows 10 / Visual Studio 2015 / CMake 3.9

Ubuntu 16.04 / GCC 7.2 / CMake 3.9