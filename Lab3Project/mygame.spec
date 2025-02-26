Name:           mygame
Version:        1.0
Release:        1%{?dist}
Summary:        A Labirynth game
Group:          Games/Entertainment 

License:        GPLv3+
URL:            https://github.com/
Source0:        mygame-1.0.tar

BuildRequires:  gcc-c++
BuildRequires:  ncurses-devel
Requires:       ncurses

%description
Labirynth game.

%prep
%setup -q

%build
make %{?_smp_mflags}

%install
mkdir -p %{buildroot}/usr/local/bin
install -m 755 mygame %{buildroot}/usr/local/bin/mygame

%files
/usr/local/bin/mygame

%changelog
* Thu Feb 26 2025 Ivan Cool <vanek@mail.ru> - 1.0-1
- Initial package.
