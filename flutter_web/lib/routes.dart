import 'package:flutter/material.dart';
import "package:flutter_web/home.dart";
import 'package:flutter_web/Second.dart';
import 'package:get_it/get_it.dart';

const String HomeRoute = '/';
const mainPath = "/";
const String SecondPageRoute = '/second';

Route<dynamic> generateRoute(RouteSettings settings) {
  var routingData = settings.name!.getRoutingData;
  switch (routingData.route) {
    case mainPath:
      return _getPageRoute(const HomePage(), settings);

    case SecondPageRoute:
      String message = routingData['message'];
      return _getPageRoute(
          SecondPage(
            message: message,
            onPressBack: () {
              locator<NavigationService>()
                  .navigateTo(HomeRoute, queryParams: {});
            },
          ),
          settings);

    default:
      return _getPageRoute(const HomePage(), settings);
  }
}

PageRoute _getPageRoute(Widget child, RouteSettings settings) {
  return RouteAnimation(child: child, routeName: settings.name);
}

class RouteAnimation extends PageRouteBuilder {
  final Widget child;
  final String? routeName;

  RouteAnimation({required this.child, required this.routeName})
      : super(
          settings: RouteSettings(name: routeName),
          pageBuilder: (
            BuildContext context,
            Animation<double> animation,
            Animation<double> secondaryAnimation,
          ) =>
              child,
          transitionsBuilder: (
            BuildContext context,
            Animation<double> animation,
            Animation<double> secondaryAnimation,
            Widget child,
          ) =>
              RotationTransition(
            turns: animation,
            child: child,
          ),
        );
}

class FadeRoute extends PageRouteBuilder {
  final Widget child;
  final String? routeName;

  FadeRoute({required this.child, required this.routeName})
      : super(
          settings: RouteSettings(name: routeName),
          pageBuilder: (
            BuildContext context,
            Animation<double> animation,
            Animation<double> secondaryAnimation,
          ) =>
              child,
          transitionsBuilder: (
            BuildContext context,
            Animation<double> animation,
            Animation<double> secondaryAnimation,
            Widget child,
          ) =>
              FadeTransition(
            opacity: animation,
            child: child,
          ),
        );
}

GetIt locator = GetIt.instance;

void setupLocator() {
  locator.registerLazySingleton(() => NavigationService());
}

class NavigationService {
  List<String> pathHistory = [];
  final GlobalKey<NavigatorState> navigatorKey = GlobalKey<NavigatorState>();

  Future<dynamic> navigateTo(String routeName,
      {required Map<String, String> queryParams}) {
    routeName = Uri(path: routeName, queryParameters: queryParams).toString();
    pathHistory.add(routeName);
    return navigatorKey.currentState!.pushNamed(routeName);
  }

  void goBack() {
    if (pathHistory.isNotEmpty) pathHistory.removeAt(pathHistory.length - 1);
    return navigatorKey.currentState!.pop();
  }

  void goBackToStart() {
    pathHistory = [];
    return navigatorKey.currentState!.popUntil((route) => route.isFirst);
  }
}

class RoutingData {
  final String route;
  final Map<String, String> _queryParameters;

  RoutingData({
    required this.route,
    required Map<String, String> queryParameters,
  }) : _queryParameters = queryParameters;

  operator [](String key) => _queryParameters[key];
}

extension StringExtension on String {
  RoutingData get getRoutingData {
    var uriData = Uri.parse(this);
    return RoutingData(
      queryParameters: uriData.queryParameters,
      route: uriData.path,
    );
  }
}
