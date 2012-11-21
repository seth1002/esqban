#/bin/sh

#/home/sqba/apps/google_appengine/dev_appserver.py --clear_datastore marinas/
/home/sqba/apps/google_appengine/dev_appserver.py marinas/

#appcfg.py upload_data --config_file=marinas/loader.py --filename=marinas.csv --kind=Marina --url=http://localhost:8080/_ah/remote_api marinas
#bulkload_client.py --filename=marinas.csv --url=http://localhost:8080/loadmarinas --kind=Marina
#appcfg.py upload_data --url=http://localhost:8080/remote_api --filename=marinas.csv marinas
#bulkloader.py --create_config --url http://localhost:8080/remote_api --filename bulkloader.yaml
#appcfg.py create_bulkloader_config --filename=bulkloader.yaml --url=http://localhost:8080/remote_api
#appcfg.py upload_data --filename=marinas.csv --kind=Marina --url=http://localhost:8080/_ah/remote_api marinas

#bulkload_client.py --filename=marinas.csv --url=http://localhost:8080/loadmarinas --kind=Marina
