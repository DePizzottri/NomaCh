## API

Принципиальных нововведений в этом слое не предполоагается. Хочется отметить что при налиции в для языка С++ в общем неплохих HTTP библиотек и фремворков бросается в глаза отсутствие удобного построения роутинга REST запросов. Конечно, OpenAPI Swagger'a и его кодогенератор решают эту проблему, но хотелось бы видеть, например некое подобие подхода Spray.

Код, демонстрирующий, как бы это могло выглядеть:
```C++
//
//METHOD protocol://login:password@subdomain.name_domain.domain:port/path1/path2/path3?query1=param1&query2=param2

Path {
    "/path1",
    GET { //match GET http://HOST:PORT/path1
        Query { //some query 
            "query1",
            [=](Param param) {//match GET http://HOST:PORT/path1?query1=PPPPPP
                return response(OK);
            }
        }, //OR
        Query { //other variant of query
            "queryx",
            "paramx" //match GET http://HOST:PORT/path1?queryx=paramx
            [=] {
                return redirect();
            }
        }
    }, //OR
    POST { //match POST http://HOST:PORT/path1
        Entity<EntityType> { //unmarshall body to certain type
            [=](EntityType entity) {
                //response
            }
        }
    }, //OR
    Method { //other methods, match XXX http://HOST:PORT/path1
        [=](MethodType method){ //bind schema to variable
            return Query<int> { //some query with typed parameter
                "query1",
                [=](int param) {//match GET http://HOST:PORT/path1?query1=NUMBER
                    //response
                }
            }
        }
    }, //OR
    PathSlash { //match XXX http://HOST:PORT/path1/path2, i.e. through next slash
        [=](PathPartType path) {
            return response(OK);
        }
    }, //OR
    PathTail { // match XXX http://HOST:PORT/path1/PATH/PATH, i.e. through the path end
        [=](PathPartType path) {
            return Queries { //zero or more all query parameters
                [=](QueryMap queries) {
                }
            }
        }
    }, //OR
    Path {
        "pathx" / "pathy" / "pathx",
        [=] {
            QueryAs<Entity> { //convert query parameters to Entity
                {"id", type<uint32_t>},
                {"name", type<string>},
                [=] (Entity entity) {
                    return response(OK);
                }
            }
        }               
    }, //OR
    encoded(gzip2){ //body in this path encoded by gzip2
        withHeader { //match current header
            "customHeader",
            [=](HeaderValue h) {
                return ExtractSOMETING { //extracts (URL, IP, PORT) or another data, not in path or http headers to use as parameter
                    [=] (SOMETHING) {
                    }
                }
            }
        }
    }
}
```

Приведенный код является валидным кодом на С++ и вполне реализуем, например при использовании техник expression-templates.