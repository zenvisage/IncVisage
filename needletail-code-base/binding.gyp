{
  "targets": [
    {
      "target_name": "incvisage",
      "sources": [ "incvisage.cc","trend.cc","heat.cc","axes.cc","sdb.cc","schema.cc","csv.cc","query.cc","swift_map.cc","swift_index.cc","binner.cc","db_utils.cc","utils.cc","config.h","segment.cc","resultobject.cc","quadrant.cc"
      ],
      "include_dirs": [
            "./EWAHBoolArray/headers","./yaml-cpp/include","/usr/local/include","/usr/include"
          ],
      "libraries":["./yaml-cpp/build/libyaml-cpp.so","./yaml-cpp/build/libyaml-cpp.so.0.5","./yaml-cpp/build/libyaml-cpp.so.0.5.3"],
      "cflags": ["-Wall", "-std=c++11"],
      'cflags_cc' : [ '-fexceptions'],
      'xcode_settings': {
        'OTHER_CFLAGS': [
          '-std=c++11'
        ],
      },
      'msvs_settings': {
        'VCCLCompilerTool': {
          'ExceptionHandling': 1 # /EHsc
        }
      },
      'configurations': {
        'Release': {
          'msvs_settings': {
            'VCCLCompilerTool': {
            'ExceptionHandling': 1,
          }
        }
      }
      },
      "conditions": [
        [ 'OS=="mac"', {
            "xcode_settings": {
                'OTHER_CPLUSPLUSFLAGS' : ['-std=c++11','-stdlib=libc++'],
                'OTHER_LDFLAGS': ['-stdlib=libc++'],
                'MACOSX_DEPLOYMENT_TARGET': '10.7' }
            }
        ]
      ]
    }
  ]
}
