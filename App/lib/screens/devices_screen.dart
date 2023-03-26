import 'package:flutter/src/widgets/framework.dart';
import 'package:flutter/src/widgets/placeholder.dart';
import 'package:flutter_reactive_ble/flutter_reactive_ble.dart';
import 'package:provider/provider.dart';

class DevicesScreen extends StatefulWidget {
  const DevicesScreen({super.key});

  @override
  State<DevicesScreen> createState() => _DevicesScreenState();
}

class _DevicesScreenState extends State<DevicesScreen> {
  final flutterReactiveBle = FlutterReactiveBle();

  @override
  void initState() {
    _scanDevices();
    super.initState();
  }

  void _scanDevices() {
    flutterReactiveBle.scanForDevices(withServices: []).listen((device) {
      print("------------------");
      print(device);
    }, onError: () {
      print("object");
    });
  }

  @override
  Widget build(BuildContext context) {
    return const Placeholder();
  }
}
