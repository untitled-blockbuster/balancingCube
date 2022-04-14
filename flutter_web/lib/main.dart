import 'package:flutter/material.dart';
import 'package:flutter_web/routes.dart';

void main() {
  runApp(const App());
}

class App extends StatefulWidget {
  const App({Key? key}) : super(key: key);

  @override
  State<App> createState() => _AppState();
}

class _AppState extends State<App> {
  _AppState() {
    setupLocator();
  }

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: '로봇고 2조 졸업작품',
      // theme: ThemeData.dark(),
      navigatorKey: locator<NavigationService>().navigatorKey,
      onGenerateRoute: generateRoute,
      initialRoute: mainPath,
    );
  }
}
