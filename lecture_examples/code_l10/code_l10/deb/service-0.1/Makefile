CFLAGS=-Os -Wall
PREFIX=debian/service/usr

service:

install: service
	install -d ${PREFIX}/bin
	install -t ${PREFIX}/bin service
	install -d ${PREFIX}/share/man/man1
	install -t ${PREFIX}/share/man/man1 service.1

clean:
	rm -rf service