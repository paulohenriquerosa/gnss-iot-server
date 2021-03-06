
from secrets import token_hex
import os
from gnss_iot_server.settings import BASE_DIR
from django.shortcuts import render, redirect, reverse
from django.http import HttpResponseRedirect, JsonResponse, HttpResponse
from django.contrib.auth.models import User
from django.contrib.auth.tokens import default_token_generator
from .forms import DeviceForm
from .models import Device


# Create your views here.
def index(request):
    """A página inicial do Gnss Iot"""
    if request.user.is_authenticated:
        return HttpResponseRedirect(reverse('gnss_iot:profile', args=[request.user.id]))
    else:
        return render(request, 'gnss_iot/index.html')


def profile(request, user_id):
    dados = User.objects.filter(id=user_id)
    context = {'dados': dados}
    return render(request, 'gnss_iot/profile.html', context=context)


def devices(request):
    devices = Device.objects.filter(owner=request.user)
    context = {'devices': devices}
    return render(request, 'gnss_iot/devices.html', context=context)


def delete_device(request, device_id):
    device = Device.objects.filter(id=device_id)
    device.delete()
    return redirect('gnss_iot:devices')


def detail_device(request, device_id):
    device = Device.objects.filter(id=device_id)
    context = {'dados': device}
    return render(request, 'gnss_iot/detail_device.html', context=context)


def edit_device(request, device_id):
    device = Device.objects.filter(id=device_id)
    device.name = request.POST['name']
    device.save()
    return redirect('gnss_iot:devices')


def new_device(request):
    new_device = Device()
    new_device.name = request.POST['name']
    new_device.owner = request.user
    new_device.token = token_hex(16).upper()
    new_device.save()
    return redirect('gnss_iot:devices')

def authentication(request, device_id):
    device = Device.objects.filter(token=device_id)
    if device:
        return HttpResponse("OK")    
    else:
        return HttpResponse("FAILED")
