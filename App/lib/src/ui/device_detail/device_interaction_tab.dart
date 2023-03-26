import 'dart:async';

import 'package:flutter/material.dart';
import 'package:flutter_reactive_ble/flutter_reactive_ble.dart';
import 'package:get/get.dart';
import '../../ble/ble_device_connector.dart';
import '../../ble/ble_device_interactor.dart';
import 'package:functional_data/functional_data.dart';
import 'package:provider/provider.dart';

import 'characteristic_interaction_dialog.dart';

part 'device_interaction_tab.g.dart';
//ignore_for_file: annotate_overrides

class DeviceInteractionTab extends StatelessWidget {
  final DiscoveredDevice device;

  const DeviceInteractionTab({
    required this.device,
    Key? key,
  }) : super(key: key);

  @override
  Widget build(BuildContext context) =>
      Consumer3<BleDeviceConnector, ConnectionStateUpdate, BleDeviceInteractor>(
        builder: (_, deviceConnector, connectionStateUpdate, serviceDiscoverer,
                __) =>
            _DeviceInteractionTab(
          viewModel: DeviceInteractionViewModel(
              deviceId: device.id,
              connectionStatus: connectionStateUpdate.connectionState,
              deviceConnector: deviceConnector,
              discoverServices: () =>
                  serviceDiscoverer.discoverServices(device.id)),
        ),
      );
}

@immutable
@FunctionalData()
class DeviceInteractionViewModel extends $DeviceInteractionViewModel {
  const DeviceInteractionViewModel({
    required this.deviceId,
    required this.connectionStatus,
    required this.deviceConnector,
    required this.discoverServices,
  });

  final String deviceId;
  final DeviceConnectionState connectionStatus;
  final BleDeviceConnector deviceConnector;
  @CustomEquality(Ignore())
  final Future<List<DiscoveredService>> Function() discoverServices;

  bool get deviceConnected =>
      connectionStatus == DeviceConnectionState.connected;

  Future<void> connect() async {
    await deviceConnector.connect(deviceId);
  }

  void disconnect() {
    deviceConnector.disconnect(deviceId);
  }
}

class _DeviceInteractionTab extends StatefulWidget {
  const _DeviceInteractionTab({
    required this.viewModel,
    Key? key,
  }) : super(key: key);

  final DeviceInteractionViewModel viewModel;

  @override
  _DeviceInteractionTabState createState() => _DeviceInteractionTabState();
}

class _DeviceInteractionTabState extends State<_DeviceInteractionTab> {
  late List<DiscoveredService> discoveredServices;

  @override
  void initState() {
    discoveredServices = [];
    super.initState();
  }

  Future<List<DiscoveredService>> discoverServices() async {
    final result = await widget.viewModel.discoverServices();
    setState(() {
      discoveredServices = result;
    });
    return result;
  }

  @override
  Widget build(BuildContext context) => CustomScrollView(
        slivers: [
          SliverList(
            delegate: SliverChildListDelegate.fixed(
              [
                Padding(
                  padding: const EdgeInsets.only(top: 16.0),
                  child: ElevatedButton(
                    onPressed: !widget.viewModel.deviceConnected
                        ? () => widget.viewModel
                            .connect()
                            .then((v) async => await discoverServices())
                        : widget.viewModel.disconnect,
                    child: Text(
                      widget.viewModel.deviceConnected
                          ? "disconnect"
                          : "connect",
                    ),
                  ),
                ),
                if (widget.viewModel.deviceConnected)
                  Consumer<BleDeviceInteractor>(
                      builder: (context, interactor, _) {
                    return _ServiceDiscoveryList(
                      deviceId: widget.viewModel.deviceId,
                      discoveredServices: discoveredServices,
                      indicator: interactor,
                    );
                  }),
              ],
            ),
          ),
        ],
      );
}

class _ServiceDiscoveryList extends StatefulWidget {
  const _ServiceDiscoveryList({
    required this.deviceId,
    required this.discoveredServices,
    required this.indicator,
    Key? key,
  }) : super(key: key);

  final String deviceId;
  final BleDeviceInteractor indicator;
  final List<DiscoveredService> discoveredServices;

  @override
  _ServiceDiscoveryListState createState() => _ServiceDiscoveryListState();
}

class _ServiceDiscoveryListState extends State<_ServiceDiscoveryList> {
  @override
  void initState() {
    super.initState();
  }

  @override
  Widget build(BuildContext context) {
    final service = widget.discoveredServices.firstWhere((service) =>
        service.serviceId ==
        Uuid.parse("653bb0e0-1d85-46b0-9742-3b408f4cb83f"));
    final txCharacteristic = service.characteristics.firstWhere(
        (characteristic) =>
            characteristic.characteristicId ==
            Uuid.parse("00c1acd4-f35b-4b5f-868d-36e5668d0929"));

    return widget.discoveredServices.isEmpty
        ? const SizedBox()
        : UnlockButton(
            interactor: widget.indicator,
            characteristic: QualifiedCharacteristic(
              characteristicId: txCharacteristic.characteristicId,
              serviceId: txCharacteristic.serviceId,
              deviceId: widget.deviceId,
            ));
  }
}

class UnlockButton extends StatefulWidget {
  const UnlockButton({
    super.key,
    required this.interactor,
    required this.characteristic,
  });

  final BleDeviceInteractor interactor;
  final QualifiedCharacteristic characteristic;

  @override
  State<UnlockButton> createState() => _UnlockButtonState();
}

class _UnlockButtonState extends State<UnlockButton> {
  late StreamSubscription<List<int>>? subscribeStream;
  late bool isUnlocked;

  Future<void> subscribeCharacteristic() async {
    subscribeStream = widget.interactor
        .subScribeToCharacteristic(widget.characteristic)
        .listen((event) {
      setState(() {
        isUnlocked = event[0] == 1 ? true : false;
      });
    });
  }

  @override
  void initState() {
    isUnlocked = false;
    subscribeCharacteristic();
    super.initState();
  }

  @override
  void dispose() {
    subscribeStream?.cancel();
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    return Padding(
      padding: const EdgeInsets.all(17),
      child: Column(
        children: [
          ElevatedButton(
            onPressed: () {
              widget.interactor.writeCharacterisiticWithResponse(
                widget.characteristic,
                isUnlocked ? "lock".codeUnits : "unlock".codeUnits,
              );
            },
            style: ElevatedButton.styleFrom(
              shape: const CircleBorder(),
              padding: const EdgeInsets.all(80),
              foregroundColor:
                  isUnlocked ? Colors.greenAccent : Colors.redAccent,
              elevation: 5.0,
            ),
            onLongPress: isUnlocked
                ? () {
                    Get.bottomSheet(
                        isDismissible: true,
                        SizedBox(
                          height: 100,
                          child: Column(
                            children: [
                              ListTile(
                                title: const Text("Keep unlocked"),
                                leading: const Icon(Icons.lock_open_outlined),
                                onTap: () => {
                                  widget.interactor
                                      .writeCharacterisiticWithResponse(
                                          widget.characteristic,
                                          "unlockforever".codeUnits),
                                  Get.back(),
                                },
                              ),
                            ],
                          ),
                        ),
                        backgroundColor: Theme.of(context)
                            .buttonTheme
                            .colorScheme
                            ?.background,
                        shape: const RoundedRectangleBorder(
                          borderRadius: BorderRadius.only(
                              topLeft: Radius.circular(15),
                              topRight: Radius.circular(15)),
                        ));
                  }
                : null,
            child: Column(
              mainAxisAlignment: MainAxisAlignment.center,
              children: [
                Icon(isUnlocked ? Icons.lock_open : Icons.lock_outline,
                    size: 80.0),
                const SizedBox(height: 8.0),
                Text(
                  isUnlocked ? "UNLOCKED" : "LOCKED",
                  style: const TextStyle(
                    fontSize: 16.0,
                    fontWeight: FontWeight.bold,
                  ),
                ),
              ],
            ),
          ),
          ElevatedButton(
            onPressed: () {
              widget.interactor.writeCharacterisiticWithResponse(
                  widget.characteristic, "alarm".codeUnits);
            },
            child: Row(
              children: const [
                Icon(Icons.notifications_on_rounded),
                Text("Ring"),
              ],
            ),
          ),
        ],
      ),
    );
  }
}
