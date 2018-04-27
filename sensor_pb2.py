# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: sensor.proto

import sys
_b=sys.version_info[0]<3 and (lambda x:x) or (lambda x:x.encode('latin1'))
from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from google.protobuf import reflection as _reflection
from google.protobuf import symbol_database as _symbol_database
from google.protobuf import descriptor_pb2
# @@protoc_insertion_point(imports)

_sym_db = _symbol_database.Default()




DESCRIPTOR = _descriptor.FileDescriptor(
  name='sensor.proto',
  package='sensor',
  syntax='proto3',
  serialized_pb=_b('\n\x0csensor.proto\x12\x06sensor\"\x1d\n\tir_sensor\x12\x10\n\x08\x64istance\x18\x01 \x01(\r\"8\n\x0c\x63olor_sensor\x12\x12\n\ncolor_code\x18\x01 \x01(\r\x12\t\n\x01x\x18\x02 \x01(\x05\x12\t\n\x01y\x18\x03 \x01(\x05\"g\n\nsensor_msg\x12&\n\tdist_data\x18\x01 \x01(\x0b\x32\x11.sensor.ir_sensorH\x00\x12*\n\ncolor_data\x18\x02 \x01(\x0b\x32\x14.sensor.color_sensorH\x00\x42\x05\n\x03msgb\x06proto3')
)




_IR_SENSOR = _descriptor.Descriptor(
  name='ir_sensor',
  full_name='sensor.ir_sensor',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='distance', full_name='sensor.ir_sensor.distance', index=0,
      number=1, type=13, cpp_type=3, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None, file=DESCRIPTOR),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  options=None,
  is_extendable=False,
  syntax='proto3',
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=24,
  serialized_end=53,
)


_COLOR_SENSOR = _descriptor.Descriptor(
  name='color_sensor',
  full_name='sensor.color_sensor',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='color_code', full_name='sensor.color_sensor.color_code', index=0,
      number=1, type=13, cpp_type=3, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='x', full_name='sensor.color_sensor.x', index=1,
      number=2, type=5, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='y', full_name='sensor.color_sensor.y', index=2,
      number=3, type=5, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None, file=DESCRIPTOR),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  options=None,
  is_extendable=False,
  syntax='proto3',
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=55,
  serialized_end=111,
)


_SENSOR_MSG = _descriptor.Descriptor(
  name='sensor_msg',
  full_name='sensor.sensor_msg',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='dist_data', full_name='sensor.sensor_msg.dist_data', index=0,
      number=1, type=11, cpp_type=10, label=1,
      has_default_value=False, default_value=None,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='color_data', full_name='sensor.sensor_msg.color_data', index=1,
      number=2, type=11, cpp_type=10, label=1,
      has_default_value=False, default_value=None,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None, file=DESCRIPTOR),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  options=None,
  is_extendable=False,
  syntax='proto3',
  extension_ranges=[],
  oneofs=[
    _descriptor.OneofDescriptor(
      name='msg', full_name='sensor.sensor_msg.msg',
      index=0, containing_type=None, fields=[]),
  ],
  serialized_start=113,
  serialized_end=216,
)

_SENSOR_MSG.fields_by_name['dist_data'].message_type = _IR_SENSOR
_SENSOR_MSG.fields_by_name['color_data'].message_type = _COLOR_SENSOR
_SENSOR_MSG.oneofs_by_name['msg'].fields.append(
  _SENSOR_MSG.fields_by_name['dist_data'])
_SENSOR_MSG.fields_by_name['dist_data'].containing_oneof = _SENSOR_MSG.oneofs_by_name['msg']
_SENSOR_MSG.oneofs_by_name['msg'].fields.append(
  _SENSOR_MSG.fields_by_name['color_data'])
_SENSOR_MSG.fields_by_name['color_data'].containing_oneof = _SENSOR_MSG.oneofs_by_name['msg']
DESCRIPTOR.message_types_by_name['ir_sensor'] = _IR_SENSOR
DESCRIPTOR.message_types_by_name['color_sensor'] = _COLOR_SENSOR
DESCRIPTOR.message_types_by_name['sensor_msg'] = _SENSOR_MSG
_sym_db.RegisterFileDescriptor(DESCRIPTOR)

ir_sensor = _reflection.GeneratedProtocolMessageType('ir_sensor', (_message.Message,), dict(
  DESCRIPTOR = _IR_SENSOR,
  __module__ = 'sensor_pb2'
  # @@protoc_insertion_point(class_scope:sensor.ir_sensor)
  ))
_sym_db.RegisterMessage(ir_sensor)

color_sensor = _reflection.GeneratedProtocolMessageType('color_sensor', (_message.Message,), dict(
  DESCRIPTOR = _COLOR_SENSOR,
  __module__ = 'sensor_pb2'
  # @@protoc_insertion_point(class_scope:sensor.color_sensor)
  ))
_sym_db.RegisterMessage(color_sensor)

sensor_msg = _reflection.GeneratedProtocolMessageType('sensor_msg', (_message.Message,), dict(
  DESCRIPTOR = _SENSOR_MSG,
  __module__ = 'sensor_pb2'
  # @@protoc_insertion_point(class_scope:sensor.sensor_msg)
  ))
_sym_db.RegisterMessage(sensor_msg)


# @@protoc_insertion_point(module_scope)
