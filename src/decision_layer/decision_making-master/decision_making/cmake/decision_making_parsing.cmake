FILE(MAKE_DIRECTORY "/home/leo/MinerBot/devel/share/decision_making/graphs" )
FILE(GLOB_RECURSE FOR_DEL /home/leo/MinerBot/devel/share/decision_making/graphs/* )
#message("delete files from /home/leo/MinerBot/devel/share/decision_making/graphs/ : ${FOR_DEL}")
if( FOR_DEL )
	FILE(REMOVE ${FOR_DEL})
endif()
execute_process(COMMAND /home/leo/MinerBot/devel/lib/decision_making_parser/decision_making_parser -pe -xml -dot -src "/home/leo/MinerBot/src/onboard/decision_layer/decision_making-master/decision_making" -dst "/home/leo/MinerBot/devel/share/decision_making/graphs" -f "/home/leo/MinerBot/src/onboard/decision_layer/decision_making-master/decision_making/src/RosExample.cpp:/home/leo/MinerBot/src/onboard/decision_layer/decision_making-master/decision_making/src/BTExample.cpp:/home/leo/MinerBot/src/onboard/decision_layer/decision_making-master/decision_making/src/ROSTask.cpp:/home/leo/MinerBot/src/onboard/decision_layer/decision_making-master/decision_making/src/FSMExample.cpp:/home/leo/MinerBot/src/onboard/decision_layer/decision_making-master/decision_making/src/HybridExample.cpp:" RESULT_VARIABLE rv)
FILE(GLOB_RECURSE CREATED_FILES RELATIVE /home/leo/MinerBot/devel/share/decision_making/graphs/ /home/leo/MinerBot/devel/share/decision_making/graphs/*.scxml /home/leo/MinerBot/devel/share/decision_making/graphs/*.btxml  /home/leo/MinerBot/devel/share/decision_making/graphs/*.taoxml)
message("   -- Created XML files:")
foreach( f ${CREATED_FILES})
     message("      -- ${f} ")
endforeach()
FILE(GLOB_RECURSE CREATED_FILES_ABS /home/leo/MinerBot/devel/share/decision_making/graphs/*.scxml /home/leo/MinerBot/devel/share/decision_making/graphs/*.btxml /home/leo/MinerBot/devel/share/decision_making/graphs/*.taoxml)
execute_process(COMMAND "python" /home/leo/MinerBot/devel/lib/decision_making_parser/decision_making_xml_parser.py -i "/home/leo/MinerBot/src/onboard/decision_layer/decision_making-master/decision_making" "/home/leo/MinerBot/devel/share/decision_making/graphs" "${CREATED_FILES_ABS}" RESULT_VARIABLE rv)
FILE(GLOB_RECURSE CREATED_FILES RELATIVE /home/leo/MinerBot/devel/share/decision_making/graphs/ /home/leo/MinerBot/devel/share/decision_making/graphs/*.dot /home/leo/MinerBot/devel/share/decision_making/graphs/*.xot)
message("   -- Created DOT files:")
foreach( f ${CREATED_FILES})
     message("      -- ${f} ")
endforeach()
FILE(GLOB_RECURSE CREATED_FILES_ABS /home/leo/MinerBot/devel/share/decision_making/graphs/*.dot)
foreach( f ${CREATED_FILES_ABS} )
	execute_process(COMMAND "dot" -q1 -Tgif -o${f}.gif  ${f} RESULT_VARIABLE rv)
endforeach()
FILE(GLOB_RECURSE CREATED_FILES RELATIVE /home/leo/MinerBot/devel/share/decision_making/graphs/ /home/leo/MinerBot/devel/share/decision_making/graphs/*.gif)
message("   -- Created GIF files:")
foreach( f ${CREATED_FILES})
     message("      -- ${f} ")
endforeach()
