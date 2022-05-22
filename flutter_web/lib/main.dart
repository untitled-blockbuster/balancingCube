import 'package:flutter/material.dart';
import 'package:flutter_web/home.dart';
import 'package:flutter_web/routeHandler.dart';
import 'package:fluro/fluro.dart';
import 'router.dart';

void main() async {
  runApp(const App());
}

class App extends StatefulWidget {
  const App({Key? key}) : super(key: key);

  @override
  State<App> createState() => _AppState();
}

class _AppState extends State<App> {
  _AppState() {
    final router = FluroRouter();
    Routes.configureRoutes(router);
    Application.router = router;
  }

  @override
  Widget build(BuildContext context) {
    return FutureBuilder(
      future: Init.instance.initialize(context),
      builder: (context, AsyncSnapshot snapshot) {
        if (snapshot.hasData == false) {
          return MaterialApp(
              home: Column(
            children: const [
              Center(
                child: Text("Loading..."),
              ),
              CircularProgressIndicator()
            ],
          )); // 초기 로딩 시 Splash Screen
        } else if (snapshot.hasError) {
          return const MaterialApp(
              home: Center(
            child: Text("Error!"),
          )); // 초기 로딩 에러 시 Error Screen
        } else {
          return snapshot.data;
        }
      },
    );
  }
}

class Init {
  Init._();

  static final instance = Init._();

  Future<Widget?> initialize(BuildContext context) async {
    // stateful widget rebuild 특징상 2번 호출되는 문제
    await Future.delayed(const Duration(milliseconds: 1000));
    MaterialApp res = MaterialApp(
      title: '로봇고 2조 졸업작품',
      home: HomePage()
    ); // 초기 로딩 완료 시 띄울 앱 첫 화면

    return res;
  }
}
