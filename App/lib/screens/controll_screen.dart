import 'package:flutter/material.dart';
import 'package:flutter/src/widgets/framework.dart';
import 'package:flutter/src/widgets/placeholder.dart';
import 'package:flutter_blue_plus/flutter_blue_plus.dart';
import 'package:get/get.dart';
import 'package:limer/screens/devices_screen.dart';

class ControllScreen extends StatefulWidget {
  const ControllScreen({super.key, required this.device});

  final BluetoothDevice device;

  @override
  State<ControllScreen> createState() => _ControllScreenState();
}

class _ControllScreenState extends State<ControllScreen> {
  FlutterBluePlus flutterBlue = FlutterBluePlus.instance;

  @override
  void initState() {
    super.initState();
    _getDevice();
  }

  Future<void> _getDevice() async {
    final device = widget.device;
    List<BluetoothService> services = await device.discoverServices();
    services.forEach((service) {
      print(service);
    });
  }

  @override
  Widget build(BuildContext context) {
    final device = widget.device;
    return Scaffold(
      body: Center(
          child: ElevatedButton(
        onPressed: () => {},
        onLongPress: () => device.disconnect(),
        style: ElevatedButton.styleFrom(
          padding: const EdgeInsets.all(
            32.0,
          ),
        ),
        child: Text(device.name),
      )),
    );
  }
}
