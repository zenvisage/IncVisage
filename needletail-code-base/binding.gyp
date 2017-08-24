{
  "targets": [
    {
      "target_name": "incvisage",
      "sources": [ "incvisage.cc","trend.cc","heat.cc","axes.cc","sdb.cc","schema.cc","csv.cc","query.cc","swift_map.cc","swift_index.cc","binner.cc","db_utils.cc","utils.cc","config.h","segment.cc","resultobject.cc","quadrant.cc"
      ],
      "include_dirs": [
            "/home/srahman7/needletail-code-base/EWAHBoolArray/headers","/home/srahman7/needletail-code-base","/home/srahman7/incvisage-cpp/needletail-code-base/yaml-cpp/include","/usr/local/include","/usr/include"
          ],
      "libraries":["/home/srahman7/incvisage-cpp/needletail-code-base/yaml-cpp/build/libyaml-cpp.so","/home/srahman7/incvisage-cpp/needletail-code-base/yaml-cpp/build/libyaml-cpp.so.0.5","/home/srahman7/incvisage-cpp/needletail-code-base/yaml-cpp/build/libyaml-cpp.so.0.5.3"],
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
